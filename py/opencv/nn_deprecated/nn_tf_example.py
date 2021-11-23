from nn_h import *
import numpy as np

train = False

mnist = tf.keras.datasets.mnist
(x_train, y_train), (x_test, y_test) = mnist.load_data()
x_test  = tf.keras.utils.normalize(x_test, axis=1)
x_train = x_train.reshape(-1, 28, 28, 1)
x_test  = x_test.reshape(-1, 28, 28, 1)

model = generate_model(2,[28]*2, 18, [1024, 128])

import pickle as p
if train:
	print('saving AI')
	model.fit(x_train, y_train, epochs=4)
	with open('weights.pickle', 'wb') as f:
		p.dump(model.get_weights(), f)

del model
model = generate_model(18, [1024, 128])
print('loading AI')
import pickle as p
with open('weights.pickle', 'rb') as f:
	model.set_weights(p.load(f))

print('done')

predictions = model.predict(x_test.reshape(-1, 28, 28, 1))

mistake_count = 0
for i in range(len(predictions)):
	if np.argmax(predictions[i]) != y_test[i]:
		mistake_count += 1
		#print(f'model mis-identified [{i}]: {np.argmax(predictions[i])} != {y_test[i]}')
print(f'model mis-identified {mistake_count}/{len(y_test)} (={mistake_count/len(y_test)*100:.2f}%)')
