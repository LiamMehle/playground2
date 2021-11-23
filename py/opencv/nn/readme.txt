issue: Neural network no better than coinflip.

What I expected:
Loss decreasing and accuracy increasing as the neural network trains, and >50% accuracy on testng data.

What happens:
Loss decreases slowly and accuracy does not improve significanlty over 50%

Parts that I tested:
Data is created and labeled correctly.
no errors are thrown unexpectedly.

The training process seems to be taking place, but no effect is seen in AI performance.


usage:
python main.py


input data:
data/ss
data/banana.png
data/crate.png

cached data:
pos.p
neg.p
data/{pos,neg}/*
