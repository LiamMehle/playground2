# KeyGen crackme

def mod_int(x):
	return x % 2**32

s = input('name: ')
x = len(s)
a = 'a'
if x > 2:
	a = s[2]


if x % 2 == 0:
	x = (x-1) * 0x9d9fd
	if x < 0:
		x += 3
	x = x//4*0xd//7
else:
	x -= 1
	if x > 2:
		x = ord(a)*0x240e//2+1

print(x)