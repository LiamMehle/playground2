from threading import Thread
from multiprocessing import Process
import multiprocessing as proc

async_types = {"none": 0, "thread": 1, "proc":2}
def aasync(target, args=[], async_type=async_types['proc']):
	if   async_type == async_types['thread']:
		thread = Thread(target=target, args=[])
	elif async_type == async_types['proc']:
		thread = Process(target=target, args=[])
	else:
		return None
	thread.start()
	return thread
		