from subprocess import check_output
from timeit import default_timer
from mpl_toolkits.mplot3d import Axes3D  # noqa: F401 unused import
from matplotlib import cm
import matplotlib.pyplot as plt
import numpy as np
from threading import Thread
import queue

ds = "data/ibm.csv"
ms = 500
N = len(open(ds).readlines())


class worker(Thread):
    def __init__(self, q):
        super().__init__()
        self.q = q

    def run(self):
        while True:
            try:
                work = self.q.get(block=False)
            except queue.Empty:
                return
            work()
            self.q.task_done()


def run(p, s, c):
    start = default_timer()
    check_output([p, ds, s, c, "0", ">", "/dev/null"]).decode()
    end = default_timer()
    return (end - start)*1000


def run_until(f, t, mi):
    i = 0
    ts = 0.
    while ts < t or i < mi:
        ts += f()
        i += 1
    return ts / i


N = len(open(ds).readlines())

step = 10
start = 7
# S = np.linspace(1, N*0.05, step)
S = np.arange(start, step + start, 1)
C = np.linspace(0., 1., step)

fpg = np.zeros((step, step))
apriori = np.zeros((step, step))
S, C = np.meshgrid(S, C)

task_q = queue.Queue()

for s in range(step):
    for c in range(step):
        def t(s=s, c=c):
            s_ = f'{int(S[s,c]):.4f}'
            c_ = f'{C[s,c]:.4f}'
            fpg[s, c] = run_until(lambda: run('build/src/fpg', s_, c_), ms, 5)
            print(s_, c_)
        task_q.put_nowait(t)

for s in range(step):
    for c in range(step):
        def t(s=s, c=c):
            s_ = f'{int(S[s,c]):.4f}'
            c_ = f'{C[s,c]:.4f}'
            apriori[s, c] = run_until(lambda: run(
                'build/src/apriori', s_, c_), ms, 5)
            print(s_, c_)
        task_q.put_nowait(t)

for _ in range(4):
    worker(task_q).start()
task_q.join()

fig = plt.figure()
ax = fig.gca(projection='3d')

ax.set_xlabel('Support')
ax.set_ylabel('Confidence')
ax.set_zlabel('Time (ms)')

surf = ax.plot_surface(S / N, C, fpg, cmap='winter',
                       linewidth=0, antialiased=False, label='fp')

surf = ax.plot_surface(S / N, C, apriori, cmap='Wistia',
                       linewidth=0, antialiased=False, label='apriori')

plt.tight_layout()
plt.show()

# plt.savefig('res.png')
# plt.close()
