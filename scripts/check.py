import numpy as np
import re
from subprocess import check_output


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
        P = tuple(rfp.findall(P))
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
        A = tuple(map(lambda x: x[1:-1], A[1:-1].split(',')))
        B = tuple(map(lambda x: x[1:-1], B[1:-1].split(',')))
        rules.append((A, B, C, S))

    return fp, rules

def dif(a, b):
    a = set(a)
    b = set(b)
    return a-b, b-a


ds = "data/ibm.csv"
N = len(open(ds).readlines())

def test(S, C):
    a = parse(check_output(["build/src/apriori", ds,
                           str(S), str(C)]).decode().split('\n'))

    b = parse(check_output(["build/src/fpg", ds,
                            str(S), str(C)]).decode().split('\n'))
    res = True
    ab, ba = dif(a[0], b[0])
    if len(ab)+len(ba) > 0:
        print(f'diff of fp = \na-b={ab}\nb-a={ba}')
        res = False
    ab, ba = dif(a[1], b[1])
    if len(ab)+len(ba) > 0:
        print(f'diff of rule = \na-b={ab}\nb-a={ba}')
        res = False
    print(f'total rules {len(a[1])}')
    return res


if test(0.01, 0.6):
    print('Ok!')
