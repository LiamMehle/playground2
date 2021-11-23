def pbar(x,xmax=20):
    msg = '\r{' + '='*x + '-'*(xmax-x) + '}'
    print(msg, end='')

def pbars(x,xmax=None):
    if xmax == None:
        xmax = [20]*len(x)
    assert len(x) == len(xmax)

    print(f'\033[{len(x)+1}F', end='')
    for v, vmax in zip(x, xmax):
        pbar(v, vmax)
        print()
    print()

pbar(2)
from time import sleep

xs = [[3,14,7,10,0,20],
      [14,7,10,0,20,3]]

for x in zip(*xs):
    pbars(x)
    sleep(1)

print()







