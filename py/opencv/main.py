if __name__ == '__main__':
	print('[init] loading libraries')

import math
import multiprocessing as mp
#import threading   	# threads are like bacon!
import time         	# keep track of when to do stuff

import cv2 as cv    	# should be self-explanatory
import numpy as np  	# should be self-explanatory
import mss          	# screenshotting
import vision
from utility import *
import ocl

if __name__ == '__main__':
	print('[init] defining functions')



def get_screenshot(mss_instance, monitor, param=cv.COLOR_BGRA2BGR):
	image      = mss_instance.grab(monitor)
	image      = np.array(image)
	image      = cv.cvtColor(image, param)
	return image

def screenshotter():
	with mss.mss() as mss_instance:
		monitor = mss_instance.monitors[0]
		get_screenshot(mss_instance, )

class ss_source:
	mss_instance = mss.mss()
	monitor = mss_instance.monitors[0]
	screenshot = None
	def _grab_new(self):
		self.screenshot = get_screenshot(self.mss_instance, self.monitor, COLOR_BGRA2GRAY)
	def get(self):


def main():
	looping  = True
	print('loading templates')
	template_names, templates = load_images_from_dir('./templates')
	print('processing templates templates')
	with mp.Pool(processes=mp.cpu_count()) as pool:
		templates = [cv.cvtColor(np.array(t),cv.COLOR_BGR2GRAY) for t in templates] # slow
		print('initializing mss')
		with mss.mss() as mss_instance:
			monitor = mss_instance.monitors[0]
			matcher = ocl.OCL()
			print('starting main loop')
			while looping:
				loop_start = float(time.time())
				image = get_screenshot(mss_instance, monitor, cv.COLOR_BGRA2GRAY)

				# <CV logic>

				matches = matcher.template_sqdiff(image, templates[0])

				
				# <CV logic>
				cv.imshow('matches', matches)
				if cv.waitKey(0) == ord('q'):
					break
				loop_end = float(time.time())
				print(f'fps = {1/(loop_end-loop_start)}')
	cv.destroyAllWindows()

if __name__ == '__main__':
	print('[init] done')
	main()




