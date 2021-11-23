import multiprocessing as mp
import pickle
from threading import Thread

import cv2 as cv
import numpy as np
import tensorflow as tf

skip_early_training = True

model = tf.keras.models.load_model('./model')

# load data
try:
	with open('pos.p', 'rb') as f:
		pos = pickle.load(f)
	with open('neg.p', 'rb') as f:
		neg = pickle.load(f)
	print('data loaded')
except FileNotFoundError:
	print('generating data')
	neg, pos = generate_samples.generate_samples()
	print('saving data')
	with open('pos.p', 'wb') as f:
		pickle.dump(pos, f)
	with open('neg.p', 'wb') as f:
		pickle.dump(neg, f)

def renderer():
	cv.waitKey(1)

renderer_t = Thread(target=renderer)
renderer_t.start()

# format data
x = np.array(list(zip(neg, pos))).reshape(-1, 64, 64, 3)
x = np.array([cv.cvtColor(_x, cv.COLOR_BGR2GRAY) for _x in x]).reshape(-1, 64, 64, 1)
y = np.array(list(zip([0]*len(neg), [1]*len(pos))), dtype=np.uint8).reshape(-1)

x_train = x[:-100]
y_train = y[:-100]
x_test  = x[-100:]
y_test  = y[-100:]

x_train = tf.keras.utils.normalize(x_train, axis=2)
x_test  = tf.keras.utils.normalize(x_test,  axis=2)

def training_step(e, b):
	print('performing {} steps with batch size of {}'.format(e, b))
	model.fit(x_train, y_train, epochs=e, workers=12, batch_size=b)
	model.save('./model')
	predictions = model.predict(x_test)

	print('testing')
	mistake_count = 0
	cv.destroyAllWindows()
	for i in range(len(predictions)):
		if abs(predictions[i] - y_test[i]) > .5:
			mistake_count += 1
			print(f'predictions[{i}] : {predictions[i]} != {y_test[i]} : y_test[{i}]')
			if mistake_count < 5:
				cv.imshow(f'predictions[{i}] : {predictions[i]} != {y_test[i]} : y_test[{i}]', x_test[i])
	print(f'model mis-identified {mistake_count}/{len(y_test)} (={mistake_count/len(y_test)*100:.2f}%)')


if not skip_early_training:
	training_step(5, 100)
	training_step(5, 64)
	training_step(5, 32)
	training_step(10, 16)

while True:
	training_step(10, 4)

renderer_t.join()
