import tensorflow as tf

# todo: refactor
# code reused (pirated) from earlier project

# only function used in main.py
# Creates a tf.keras sequential
# model with a Conv2D first layer, followed by Dense
# layers throughout. Set up for binary categorization.
def create_model(input_shape, channels: int, conv_size: list[int,int], neuron_counts: list[int]):
	model = tf.keras.models.Sequential()
	layer = tf.keras.layers
	model.add(layer.Conv2D(channels, conv_size, activation=tf.nn.relu, input_shape=input_shape))
	#model.add(layer.Flatten())
	for neuron_count in neuron_counts[:-1]:
		model.add(layer.Dense(neuron_count, input_dim=2, activation=tf.nn.relu))
	model.add(layer.Flatten())
	model.add(layer.Dense(neuron_counts[-1],  activation=tf.nn.sigmoid))

	model.compile(optimizer=tf.keras.optimizers.Adam(learning_rate=10**-6),
                  #loss='sparse_categorical_crossentropy',
                  #loss=tf.keras.losses.BinaryCrossentropy(from_logits=False),
				  loss='binary_crossentropy',
                  metrics=['accuracy'])

	return model

# -------------------------------------------------------------------------------------------------

def generate_model(*args):
	if len(args) == 0:
		args = (18, [1024*8,1024*8])
	elif len(args) != 2:
		raise ValueError("generate_model() missing 2 required positional arguments: 'conv_size' and 'neuron_counts'")
	# 28² of handwritten digits 0-9
#	mnist = tf.keras.datasets.mnist
#
#	(x_train, y_train), (x_test, y_test) = mnist.load_data()
#
#	x_train = tf.keras.utils.normalize(x_train, axis=1)
#	x_train = x_train.reshape(-1, 28, 28, 1)
#	x_test  = tf.keras.utils.normalize(x_train, axis=1)

	model = create_model(*args)
	#model.fit(x_train, y_train, epochs=5)
	return model


def print_errors(model, x_test, y_test):
    t0 = float(time.time())
    predictions = model.predict(x_test.reshape(-1, 28, 28, 1))
    t1 = float(time.time())
    prediction_time = t1-t0

    mistake_count = 0
    for i in range(len(predictions)):
        if np.argmax(predictions[i]) != y_test[i]:
            mistake_count += 1
    print(f'model mis-identified {mistake_count}/{len(y_test)} (={mistake_count/len(y_test)*100:.2f}%)')
    print(f'time taken to predict {len(y_test)} is {prediction_time}\n{prediction_time/len(y_test)*1000:.2f}ms\n{len(y_test)/prediction_time/1000:.2f}k predictions/s')
