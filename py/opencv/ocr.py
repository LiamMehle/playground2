import time

import cv2 as cv
import mss  # screenshotting
import numpy as np
import pytesseract
from pynput.mouse import Button, Controller
from pynput.keyboard import Key, Listener
from pynput import keyboard
import multiprocessing as mp
from framelimiter import *
from threading import Thread


def get_screenshot(mss_instance, monitor, param=cv.COLOR_BGRA2BGR):
	image      = mss_instance.grab(monitor)
	image      = np.array(image)
	#image      = cv.cvtColor(image, param)
	return image


mouse = Controller()
frame_limiter = FrameLimiter()
window_size = [50]*2



170/360*255

hsv_filter_darktext = (np.array([0,0,180]),np.array([255,255,255]))
hsv_filter_whitetext = (np.array([0,0,0]),np.array([255,255,128]))

mss_instance = mss.mss()
monitor = mss_instance.monitors[0]
frame_limiter.wait(1/1000)
def grab_text() -> None:
	print()
	# get data
	image     = get_screenshot(mss_instance, monitor)
	pos       = mouse.position

	# crop out sub-image
	left      = pos[0]-window_size[0]//2
	right     = left+window_size[0]
	top       = pos[1]-window_size[1]//2
	bottom    = top+window_size[1]
	image = image[top:bottom,left:right]

	# work on cropped image for perf reasons
	image     = cv.cvtColor(image, cv.COLOR_BGRA2BGR)
	if image.mean() > 160:
		hsv_filter = hsv_filter_darktext
	else:
		hsv_filter = hsv_filter_whitetext

	image_hsv = cv.cvtColor(image, cv.COLOR_BGR2HSV)
	mask      = cv.inRange(image_hsv, hsv_filter[0], hsv_filter[1])
	text      = pytesseract.image_to_string(mask)
	image     = cv.bitwise_and(image, image, mask=mask)

	# show image
	cv.imshow('cropped image', mask)
	cv.waitKey(1)
	# frame limiter to reduce resource pressure
	print('-'*20)
	print(text)

	print(f'fps   ={frame_limiter.fps:.2f}')
	print(f'work  ={frame_limiter.work*100:.2f}%')
	print(f'maxfps={frame_limiter.max_fps:.2f}')
	#frame_limiter.wait(1)

process_queue = []

def grab_text_async():
	p = Thread(target=grab_text)
	p.start()
	process_queue.append(p)

def grab_text_async_alot():
	return
	for _ in range(1000):
		grab_text_async()


with keyboard.GlobalHotKeys({'g': grab_text,'h': grab_text_async_alot}) as listener:
	listener.join()
	

#img = cv.imread('img.png')
#text = pytesseract.image_to_string(img)
#print(text)
#cv.imshow('image', img)
#cv.waitKey(0)
