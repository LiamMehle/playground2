# a BTDB banana finding neural network script
# aim is to create a banana(crate)-image-finding feed-forward perceptron
# as a substitute to template matching

import multiprocessing as mp
import pickle

import cv2 as cv
import numpy as np

import generate_samples
from nn_h import *

# generate data
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

# generate model
print('creating model')
# from nn_h.py
#def create_model(input_shape, channels: int, conv_size: list[int,int], neuron_counts: list[int]):
model = create_model(input_shape=(64,64,1), channels=2, conv_size=[52]*2,
                     neuron_counts=[16]*5 + [128, 1])


x = np.array(list(zip(neg, pos))).reshape(-1, 64, 64, 3)
x = np.array([cv.cvtColor(_x, cv.COLOR_BGR2GRAY) for _x in x]).reshape(-1, 64, 64, 1)
y = np.array(list(zip([0]*len(neg), [1]*len(pos))), dtype=np.uint8).reshape(-1)

x_train = x[:-100]
y_train = y[:-100]


x_test  = x[-100:]
y_test  = y[-100:]

# treat the data
x_train = tf.keras.utils.normalize(x_train, axis=2)
x_test  = tf.keras.utils.normalize(x_test,  axis=2)


# train
print('training')
model.fit(x_train, y_train, epochs=15, workers=12, batch_size=100)
model.save('./model2')
predictions = model.predict(x_test)

print('testing')
mistake_count = 0
for i in range(len(predictions)):
	if abs(predictions[i] - y_test[i]) > .5:
		mistake_count += 1
		print(f'predictions[{i}] : {predictions[i]} != {y_test[i]} : y_test[{i}]')
		if mistake_count < 5:
			cv.imshow(f'predictions[{i}] : {predictions[i]} != {y_test[i]} : y_test[{i}]', x_test[i])
			cv.waitKey(1)
print(f'model mis-identified {mistake_count}/{len(y_test)} (={mistake_count/len(y_test)*100:.2f}%)')


# save


# load
# use
