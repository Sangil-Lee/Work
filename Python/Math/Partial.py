
from functools import *

def add(a, b, c):
    return 100 *a + b*10 + c

part = partial(add, c=2, b=1)

print(part(3))
