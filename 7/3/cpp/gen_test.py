import io
import subprocess

import numpy as np
import scipy.stats as sts
import os


def gen(N):
    graph = []
    for i in range(N):
        graph.append((sts.norm.rvs(), sts.norm.rvs()))

    return graph


def get_err(N, n_iter):
    err_list = []

    for _ in range(n_iter):
        g = gen(N)
        f = io.StringIO()

        f.write(f"{N}\n")
        for u, v in g:
            f.write(f"{u} {v}\n")

        S = f.getvalue()

        test_proc = subprocess.Popen(
            [os.environ['SALESMAN_PROG']], stdin=subprocess.PIPE, stdout=subprocess.PIPE)
        out, _ = test_proc.communicate(S.encode())
        ans1, ans2 = list(map(float, out.decode().split("\n")[:2]))
        err = abs(ans2 - ans1)
        err_list.append(err)

    print(f"N={N}"
          f" mean_abs_err={np.mean(err_list):.4f}"
          f" quantile_abs_err={np.quantile(err_list, 0.5):.4f}"
          f" max_abs_err={max(err_list):.4f}"
          f" min_abs_err={min(err_list):.4f}"
          f" rmse={np.sqrt(np.mean(np.array(err_list)**2))}")


np.random.seed(123)
for N in range(4, 12):
    get_err(N, 30)
