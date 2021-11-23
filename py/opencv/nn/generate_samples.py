import cv2 as cv
import numpy as np
import os
import multiprocessing as mp

ss_dir     = './data/ss/'
data_dir   = './data/'
neg_dir    = './data/neg/'
pos_dir    = './data/pos/'
banana_files = {'./banana.png', './crate.png'}
sample_size = 64

def blit(dst: np.array, src: np.array, dx: int, dy: int) -> np.array:
	y1, y2 = dy, dy + src.shape[0]
	x1, x2 = dx, dx + src.shape[1]
	try:
		# try taking advantage of alpha channel
		alpha_s = src[:, :, 3] / 255.0
	except IndexError:
		# fallback brute force
		alpha_s = np.zeros(src.shape[:2])
		for x in range(src.shape[1]):
			for y in range(src.shape[0]):
				for c in range(src.shape[2]):
					if src[y,x,c] != 0:
						alpha_s[y,x] = 1
						break


	alpha_d = 1.0 - alpha_s

	for c in range(0, 3):
		try:
			src_c = src[:, :, c]
			_src = alpha_s * src_c

			dst_c = dst[y1:y2, x1:x2, c]
			_dst = alpha_d * dst_c
			dst[y1:y2, x1:x2, c] = (_src + _dst)
		except IndexError as e:
			break
	return dst


def generate_neg(basename: str, img: np.array, do_save: bool = False):
	ret = []
	for x in range(img.shape[1]//sample_size):
		for y in range(img.shape[0]//sample_size):
			basename = basename.split('.')[0]
			filename = f'{basename}-{x}-{y}.jpeg'
			sample = img[y*sample_size:(y+1)*sample_size,
			             x*sample_size:(x+1)*sample_size]
			if do_save:
				cv.imwrite(os.path.join(neg_dir, filename), sample)
			ret.append(sample)
	return ret


def generate_pos(basename: str, img: np.array, banana: np.array, do_save: bool = False):
	ret = []

	for x in range(img.shape[1]//sample_size):
		for y in range(img.shape[0]//sample_size):
			basename = basename.split('.')[0]
			filename = f'{basename}-{x}-{y}.jpeg'
			sample = img[y*sample_size:(y+1)*sample_size,
			             x*sample_size:(x+1)*sample_size]

			# for each sample (loop ellision)
			x_cnt = sample.shape[1] - banana.shape[1]
			y_cnt = sample.shape[0] - banana.shape[0]
			dx = np.random.randint(x_cnt)
			dy = np.random.randint(y_cnt)
			sample = blit(sample, banana, dx, dy)
			if do_save:
				cv.imwrite(os.path.join(pos_dir, filename), sample)
			ret.append(sample)
	return ret

def _generate_samples(filename: str):
	img = cv.imread(os.path.join(ss_dir, filename), 1)
	bananas = []
	for banana_file in banana_files:
		banana_file = os.path.join(data_dir, banana_file)
		f = cv.imread(banana_file, 1)
		bananas.append(f)

	# do_save=True as a workaround to mp.Pool.map() giving unexpectedly shouffled data
	# will collect saved images later
	neg = generate_neg(filename, img, do_save=True)
	index = np.random.randint(0,2)
	pos = generate_pos(filename, img, bananas[index], do_save=True)
	return (neg, pos)

# candidates for code elision as they're near-identical, but used in mp.Pool.map()
def collect_neg(filename):
	return cv.imread(os.path.join(neg_dir, filename), 1)

def collect_pos(filename):
	return cv.imread(os.path.join(pos_dir, filename), 1)

def generate_samples():
	with mp.Pool(processes=16) as pool:
		filenames = os.listdir(ss_dir)
		pool.map(_generate_samples, filenames)
		neg = pool.map(collect_neg, [f for f in os.listdir(neg_dir)])
		pos = pool.map(collect_pos, [f for f in os.listdir(pos_dir)])
	return (neg, pos)

if __name__ == '__main__':
	generate_samples()
	print('done')