import random
import io
from subprocess import Popen, PIPE


def gen_test(N, max_n):
    outp = io.StringIO()
    is_add = 0
    arr = [10]
    outp.write(f"{N}\n")
    outp.write("10 0\n")
    size = 1

    for i in range(N):
        is_add = random.randint(0, 1)

        if size < 2:
            is_add = 1
        if is_add:
            t1 = random.randint(1, max_n)
            arr.append(t1)
            size += 1
            t2 = random.randint(1, size) - 1
        else:
            idx = random.randint(1, size) - 1
            t1 = -arr[idx]
            arr.remove(-t1)

            size -= 1
            t2 = random.randint(1, size) - 1
        outp.write(f"{t1} {t2}\n")

    return outp.getvalue()


def run_prog(inp):
    p = Popen("cmake-build-debug/2", stdin=PIPE, stdout=PIPE)
    out, _ = p.communicate(inp.encode())
    return p.returncode

def test_1():
    for _ in range(10):
        inp = gen_test(4, 4)
        code = run_prog(inp)
        if code != 0:
            print(code)
            print(inp)
            break


test_1()
