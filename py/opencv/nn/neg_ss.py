# screenshotting utility
# takes screenshot on 'g' press

import os
from threading import Thread

import cv2 as cv
import mss  # screenshotting
import numpy as np
from pynput import keyboard
save_dir = './data/neg/'

def get_screenshot(mss_instance, monitor, param=cv.COLOR_BGRA2BGR):
	image      = mss_instance.grab(monitor)
	image      = np.array(image)
	return image


def grab_screen() -> None:
	print('---------')
	print('capturing')
	image        = get_screenshot(mss_instance, monitor)
	print('captured')
	filenames    = os.listdir(save_dir)
	file_numbers = [int(x.split('.')[0]) for x in filenames]
	try:
		max_number = max(file_numbers)+1
	except ValueError as e:
		max_number = 0
	filename     = f'{max_number}.jpeg'
	cv.imwrite(os.path.join(save_dir,filename), image)
	print(f'saved as {filename}')

mss_instance = mss.mss()
monitor = mss_instance.monitors[0]

with keyboard.GlobalHotKeys({'g': grab_screen}) as listener:
	listener.join()
