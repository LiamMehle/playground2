def midpoint( a, b, bias = .5 ):
	return ( a*(1 - bias) + b*(bias) )

def print_len( len ):
	if len >= 1:
		print(f'{len}m')
	elif len < 1:
		print(f'{len*1000}mm')
	elif len < 1:
		print(f'{len*1000_000}um')


M = 10**6
c = 299_792_458
freq = [ midpoint( 137*M, 174*M, .7 ) , 446.3*M ]

wavelen  = [ x/c for x in freq ]
monopole = [ x/2 for x in wavelen ]

antena_lens = list(range(2))
antena_lens[0] = [ monopole[0]/x for x in range(1, 100) ]
antena_lens[1] = [ monopole[0]/x for x in range(1, 100) ]

max_ln = 10
print_c = 0
for len in antena_lens[0]:
	if len in antena_lens[1]:
		print_c += 1
		print_len(len)
	if print_c == max_ln:
		break

quit(0)

for( int j = 0; j < count[i]; j++ )

const int array_len = (int)strlen(array);
for( int i = 0; i < count_len; i++ )
puts("Vnesite niz:");