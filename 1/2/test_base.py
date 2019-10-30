import pytest
import random
import io
from subprocess import Popen, PIPE


def gen_test(N, delta):
    outp = io.StringIO()
    last_t = 0

    outp.write(f"{N}\n")

    for i in range(N):
        t1 = random.randint(last_t, last_t + delta)
        t2 = random.randint(t1 + 2, t1 + delta)
        last_t = t1+1
        outp.write(f"{t1} {t2}\n")

    return outp.getvalue()


def run_prog(inp):
    p = Popen("cmake-build-debug/2", stdin=PIPE, stdout=PIPE)
    out, _ = p.communicate(inp.encode())
    return out.decode()


def run_prog_stl(inp):
    p = Popen("cmake-build-debug/2_stl", stdin=PIPE, stdout=PIPE)
    out, _ = p.communicate(inp.encode())
    return out.decode()


def test_1():
    for _ in range(100):
        inp = gen_test(1000, 2)
        assert run_prog(inp) == run_prog_stl(inp), f"input val:\n{inp}"
