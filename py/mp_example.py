import multiprocessing as mp
import time


q = mp.Queue()

q.put(2)
q.put('hi, hello')

print(q.get())
print(q.get())

















quit()

def do_work(a, b, out):
	out.put(a+b+2)

do_work(1, 2, q)

print(q.get())































quit()


def do_a_lot_of_work(a, b, out):
	time.sleep(2)
	do_work(a,b,out)

p1 = mp.Process(do_a_lot_of_work, 1, 2, q)
p2 = mp.Process(do_a_lot_of_work, 2, 1, q)

p1.join()
p2.join()


