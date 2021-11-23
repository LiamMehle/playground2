
import cv2 as cv    	# should be self-explanatory
import numpy as np  	# should be self-explanatory


def threshold_template(image, template, threshold):
	for _ in range(1000):
		matches    = cv.matchTemplate(image, template, cv.TM_SQDIFF_NORMED)
	_,locations = cv.threshold(matches, 1-threshold, 255, cv.THRESH_BINARY)
	#locations = np.where(matches.get() >= 1-threshold)
	locations  = np.array(zip(*locations.get()[::-1]))
	#locations = cull_locs(matches, locations, min_dist)           # incredibly compute-heavy with GIL
	return locations