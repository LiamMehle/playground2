# a BTDB banana finding neural network script
# aim is to create a banana(crate)-image-finding feed-forward perceptron
# as a substitute to template matching

import multiprocessing as mp
import pickle

import cv2 as cv
import numpy as np

import generate_samples
from nn_h import *

# aquire data
import tensorflow as tf
mnist = tf.keras.datasets.mnist
(x_train, y_train), (x_test, y_test) = mnist.load_data()
x_test  = tf.keras.utils.normalize(x_test, axis=1)
x_train = x_train.reshape(-1, 28, 28, 1)
x_test  = x_test.reshape(-1, 28, 28, 1)



# generate model
print('creating model')
# from nn_h.py
#def create_model(input_shape, channels: int, conv_size: list[int,int], neuron_counts: list[int]):
model = create_model(input_shape=(28,28,1), channels=2, conv_size=[20]*2,
                     neuron_counts=[1024, 128, 10])


# treat the data
x_train = tf.keras.utils.normalize(x_train, axis=1)
x_test  = tf.keras.utils.normalize(x_test,  axis=1)


# train
print('training')
model.fit(x_train, y_train, epochs=15, workers=12, use_multiprocessing=True, batch_size=100)
predictions = model.predict(x_test)

print('testing')
mistake_count = 0
for i in range(len(predictions)):
	if predictions[i] != y_test[i]:
		mistake_count += 1
		#cv.imshow(f'predictions[{i}] : {predictions[i]} != {y_test[i]} : y_test[{i}]', x_test[i])
		print(f'predictions[{i}] : {predictions[i]} != {y_test[i]} : y_test[{i}]')
		#cv.waitKey(0)
print(f'model mis-identified {mistake_count}/{len(y_test)} (={mistake_count/len(y_test)*100:.2f}%)')

# save


# load
# use
