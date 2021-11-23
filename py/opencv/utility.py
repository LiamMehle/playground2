import math
import multiprocessing as mp
import os
import threading  # threads are like bacon!
import time
import cv2 as cv


class Thread:
	ret_q = None
	t     = None

	def __init__(this, f, *args, type=0):
		def call(f, ret_q, *args):
			ret_q.put(f(*args))

		this.ret_q = mp.Queue()
		if type == 0:
			this.t = threading.Thread(target=call, args=[f, this.ret_q, *args])	
		else:
			this.t = mp.Process(      target=call, args=[f, this.ret_q, *args])
		this.t.start()

	def join(this):
		this.t.join()
		return this.ret_q.get()

def draw_rect(dst, template, loc, c=(0,255,0)) -> None:
	needle_w = template.shape[1]
	needle_h = template.shape[0]
	top_left = loc
	bottom_right = (loc[0]+needle_h, loc[1]+needle_w)
	cv.rectangle(dst, top_left, bottom_right, color=c, thickness=2, lineType=cv.LINE_4)

def load_images_from_dir(directory) -> tuple[list[str],list[np.Array]]:
	image_names = [f for f in os.listdir(directory) if not f in ['.', '..']]
	images      = [cv.imread(os.path.join(directory, f)) for f in image_names]
	return (image_names, images)

def cull_locs(matches, locations, min_dist) -> list:
	no_deletes = False
	while not no_deletes:
		no_deletes = True
		for loc1 in locations:
			for loc2 in locations:
				if not loc1 in locations:
					break
				if loc2 == loc1:
					continue
				dx = loc1[1]-loc2[1]
				dy = loc1[0]-loc2[0]
				dist = math.sqrt(dx*dx + dy*dy)
				if dist <= min_dist:
					no_deletes = False
					if matches[loc1[1],loc1[0]] > matches[loc2[1],loc2[0]]:
						locations.remove(loc2)
					else:
						locations.remove(loc1)
	return locations

class Profiler:
	name = None
	start = None
	factor = None
	def __init__(self, name, factor=1):
		self.name = name
		self.factor = factor
		self.start = float(time.time())

	def __enter__(self):
		return self

	def __del__(self):
		time_elapsed = float(time.time()) - self.start
		print(f'[{self.name}]: {time_elapsed*1000/self.factor:.2f} ms')
	
	def __exit__(self, *other):
		pass

def profile(f):
	def wrapper():
		with Profiler(__name__):
			f()
	return wrapper
