# neural network that should be able to help speed up image processing
import numpy as np


# each list containts a neuron's weights
# conveniently, the matrix product of W x i = n
# where W - weights
#       i - inputs
#       n - neuron activations before bias
#inputs  = [1.2, 5.1, 2.1]
#weights = [[3.1, 2.1, 8.7],
#           [3.1, 2.1, 8.7],
#           [3.1, 2.1, 8.7]]
#biases = [3]*3


def init_values(layer_count: list):
	weights = [None] * (len(layer_count)-1)
	biases = [None] * (len(layer_count)-1)
	for i in range(len(layer_count)-1):
		weights[i] = np.random.randn(layer_count[i+1], layer_count[i])
		biases[i] = np.zeros(layer_count[i+1])

	return weights, biases

def softmax(input) -> np.array:
	exp = np.exp(input)
	return exp / np.sum(exp)

def forward(inputs, weights, biases) -> np.array:
	output = inputs
	#print(f'inputs: {inputs}')
	for i in range(len(layer_count)-1):
		output = np.dot(weights[i], output) + biases[i]
		output = output - np.max(output, axis=0, keepdims=True)
		output = softmax(output)
		#print(f'      : {output}')
	#print(f'result: {output}')
	return output


def generate_sample(i: float) -> (float,float,float):
	return np.array(i, i**2), np.array(i**3)

def loss(result: np.array, expected: np.array) -> np.array:
	error = result - np.array(expected)
	return np.sum(error*error)

layer_count = np.array([2,3,3,1])
inputs  = np.random.randn(layer_count[0])


nn = init_values(layer_count)
out_layer = forward(inputs, *nn)
out_layer = softmax(out_layer)

training_in  = []
training_out = []
for x in np.arange(0,1,.01):
	training_in.append([x, x**2])
	training_out.append([x**1.5])


def get_nn_loss(weights, biases, training_in, training_out):
	losses = []
	for i,o in zip(training_in, training_out):
		losses.append(loss(forward(i, weights, biases), o))
	return np.array(losses).mean()

weights, biases = nn
weights = np.array(weights)
biases = np.array(biases)


def get_diff(weights, biases, training_in, training_out):
	original_weights = weights.copy()
	diff_weights     = weights.copy()
	original_biases  = biases.copy()
	diff_biases      = biases.copy()
	original_loss    = get_nn_loss(weights, biases, training_in, training_out)
	delta = 0.001

	for s in range(weights.shape[0]):
		for y in range(weights[s].shape[0]):
			biases[s][y]        += delta
			diff_biases[s][y] = (get_nn_loss(weights, biases, training_in, training_out) - original_loss)/delta
			biases[s][y]        -= delta
			for x in range(weights[s].shape[1]):
				weights[s][y,x]      += delta
				diff_weights[s][y,x] = (get_nn_loss(weights, biases, training_in, training_out) - original_loss)/delta
				weights[s][y,x]      -= delta
	return diff_weights, diff_biases


def apply_diff(weights, biases, diff_weights, diff_biases):
	for s in range(weights.shape[0]):
		for y in range(weights[s].shape[0]):
			biases[s][y] -= diff_biases[s][y]
			for x in range(weights[s].shape[1]):
				weights[s][y,x] -= diff_weights[s][y,x]
	return weights, biases



for i in range(100):
	diff_weights, diff_biases = get_diff(weights, biases, training_in, training_out)
	print(diff_weights)
	print(diff_biases)
	weights, biases = apply_diff(weights, biases, diff_weights, diff_biases)
	print(f'[{i}]: {get_nn_loss(weights, biases, training_in, training_out):.4f}')