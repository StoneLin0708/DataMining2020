from subprocess import check_output
from timeit import default_timer
import preprocess
import pandas as pd
import re
import argparse


def parse(o):
    o = list(filter(lambda x: len(x) > 0, o))
    rfp = re.compile(r'([^,]+)')
    i = 0
    fp = []
    if len(o) == 0:
        return [], []
    while o[i][0] != '{':
        s = o[i]
        S, P = s.split(':')
        S = int(S)
        P = tuple(filter(lambda x: len(x) > 0, map(
            lambda x: x.strip()[2:-2], rfp.findall(P))))
        fp.append((P, S))
        i += 1
        if i >= len(o):
            return fp, []

    rules = []
    for s in o[i:]:
        a = s.split('->')
        A = a[0].strip()
        a = a[1].split('=')
        B = a[0].strip()
        C, S = a[1].split()
        C = float(C)
        S = float(S)
        A = tuple(map(lambda x: x.strip()[1:-1], A[1:-1].split(',')))
        B = tuple(map(lambda x: x.strip()[1:-1], B[1:-1].split(',')))
        rules.append((A, B, C, S))

    return fp, rules


preprocess.movie(
    top_movie=0.3,
    max_movie=15,
    max_user=5000,
)

parser = argparse.ArgumentParser()
parser.add_argument('-s', type=float, default=0.005)
parser.add_argument('-c', type=float, default=0.5)
args = parser.parse_args()

start = default_timer()
r = check_output(['build/src/fpg', 'data/movie_data.csv',
                  str(args.s), str(args.c), '0']).decode().split('\n')
end = default_timer()
fp, rules = parse(r)


mv = pd.read_csv('data/movies.csv')

mvd = {}
for i, j in zip(mv['movieId'], mv['title']):
    mvd[i] = j

for s, m in fp:
    try:
        print(f'{m} : ' + ','.join(map(lambda x: mvd[x], map(int, s))))
    except:
        print(s, m)
    

for a, b, c, s in rules:
    a = ','.join(map(lambda x: mvd[x], map(int, a)))
    b = ','.join(map(lambda x: mvd[x], map(int, b)))
    print(f'c={c} s={s} \n   {a} \n-> {b}\n')

print(f'time : {end-start:.4f} sec')
