import subprocess
import unittest
from collections import deque
import random
import io
import tqdm


def gen_test_str(N):
    inp = io.StringIO()
    d = deque()
    inp.write(f"{N}\n")
    for i in range(N):
        c = random.randint(2, 3)
        v = random.randint(1, N)

        if c == 3:
            d.appendleft(v)
            inp.write(f"{c} {v}\n")
        else:
            if len(d) == 0:
                ret_val = -1
            else:
                ret_val = d.pop()
            inp.write(f"{c} {ret_val}\n")
    return inp.getvalue()


class TestDeque(unittest.TestCase):
    def test_upper(self):
        for _ in tqdm.tqdm(range(10)):
            N = random.randint(1, 30)
            inp_val = gen_test_str(N)
            test_proc = subprocess.Popen(["cmake-build-debug/1"], stdin=subprocess.PIPE, stdout=subprocess.PIPE)
            out, _ = test_proc.communicate(inp_val.encode())

            test_proc = subprocess.Popen(["cmake-build-debug/1_stl"], stdin=subprocess.PIPE, stdout=subprocess.PIPE)
            out1, _ = test_proc.communicate(inp_val.encode())
            self.assertEqual(out, out1, msg=inp_val)


if __name__ == '__main__':
    unittest.main()

"""
16
2 -1
2 -1
3 1
3 15
3 11
2 1
2 15
3 8
2 11
3 16
3 1
3 16
2 8
2 16
2 1
2 16
"""
