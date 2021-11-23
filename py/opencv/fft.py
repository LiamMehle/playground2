import cmath

def fft(P):
	n = len(P)
	if n== 1:
		return P
	w = cmath.exp(complex(0,2*cmath.pi/n))
	Pe, Po=P[0::2],P[1::2]
	ye, yo = fft(Pe), fft(Po)
	y = [0]*n
	for i in range(n//2):
		y[i]      = ye[i] + (w**i)*yo[i]
		y[i+n//2] = ye[i] - (w**i)*yo[i]
	return y

def ifft(P):
	n = len(P)
	if n== 1:
		return P
	w = cmath.exp(complex(0,-2*cmath.pi/n)) * 1/n
	Pe, Po=P[0::2],P[1::2]
	ye, yo = ifft(Pe), ifft(Po)
	y = [0]*n
	for i in range(n//2):
		y[i]      = ye[i] + (w**i)*yo[i]
		y[i+n//2] = ye[i] - (w**i)*yo[i]
	return y

x = [0,1,0]

print(fft(x))