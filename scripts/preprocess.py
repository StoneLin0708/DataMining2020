import pandas as pd
import numpy as np


def movie(top_movie=0.2, max_movie=10, max_user=2000):
    d = np.array(pd.read_csv('data/ratings.csv'))[:, :3]

    data = {int(i): [] for i in set(d[:, 0])}

    for [u, m, r] in d:
        data[u].append([int(m), r])

    for k, v in data.items():
        r = sorted(v, key=lambda i: i[1])[::-1]
        data[k] = [i[0] for i in r[:int(len(r)*top_movie)]][:max_movie]

    data = sorted(data.items(), key=lambda i: i[0])[:max_user]
    open('data/movie_data.csv',
         'w').writelines('\n'.join([','.join(map(str, [k]+v)) for k, v in data]))
