COUNT_LEN = 128

def main():
	array = [0] * 1024

	print('Vnesite niz:')
	array = input()

	array_len = len(array)

	count_len = COUNT_LEN;
	count = [0] * COUNT_LEN
	# counting sort

	for i in range( array_len ):
		count[ ord(array[i]) ] += 1

	print('Razvrščen niz je:')
	for i in range(count_len):
		for j in range(count[i]):
			print( chr(i), end='' )

	print()

main() # v C/C++ se main prvo izvede*


# *tenchnically not true, but for us it might as 