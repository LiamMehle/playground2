# printing is considered I/O, so I made a printer thread
import multiprocessing as mp
from threading import Thread
import time

def printer(q):
	while True:
		msg = q.get()
		if msg == None:
			return
		print(msg)

def aprint(msg=''):
	print_queue.put(msg)

def stop_printer():
	aprint(None)
	printer_thread.join()

print_queue    = mp.Queue()
printer_thread = Thread(target=printer, args=[print_queue])
printer_thread.start()

last_perf_log_time = time.time()
last_msg = None
def perf_log(msg):
	global last_perf_log_time
	global last_msg
	time_str = f'[{time.time()-last_perf_log_time:>7.3f}]: '
	if last_msg != None:
		aprint(f'{time_str}{last_msg}')
	last_msg = msg
	last_perf_log_time = time.time()
