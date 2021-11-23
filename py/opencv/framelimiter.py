import time

def avg(*x):
	return sum([*x])/len([*x])

#todo: add perf stats

class FrameLimiter:
	last_wait  = float(time.time())
	correction = 0

	max_fps = None
	fps     = None
	work    = None

	def wait(self, frame_time) -> None:
		time_elapsed = float(time.time())-self.last_wait
		sleep_for = frame_time-time_elapsed + self.correction
		if sleep_for > 0:
			before_sleep        = float(time.time())
			time.sleep(sleep_for)
			after_sleep         = float(time.time())
			time_spent_sleeping = after_sleep - before_sleep
			self.correction     = avg(self.correction, sleep_for - time_spent_sleeping)
		self.last_wait = float(time.time())
	
		self.max_fps = 1/time_elapsed
		self.fps     = 1/(time_elapsed+sleep_for)
		self.work    = self.fps/self.max_fps