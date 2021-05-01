class main {
	private static void print( String s ) {
		System.out.println( s );
	}
	private static void print_lines( String s[] ) {
		for( int i = 0; i < s.length; i++ )
			print( s[i] );
	}
	private static String int_arr_to_str( int value[] ) {
		String s = "[ ";
		for( int i = 0; i < value.length; i++ )
			s += String.valueOf(value[i]) + " ";
		s += "]";
		return s;
	}
	public static void main( String args[] ) {
		final int hits = Integer.parseInt(args[0]);
		final int misses = Integer.parseInt(args[1]);
		final int total_shots = hits + misses;
		final float acc = (float) hits / total_shots;
		final float hits_per_miss = (float)hits/misses;
		String output[] = {
			"hits:          " + String.valueOf(hits),
			"misses:        " + String.valueOf(misses),
			"total_shots:   " + String.valueOf(total_shots),
			"acc:           " + String.valueOf(acc),
			"hits_per_miss: " + String.valueOf(hits_per_miss)
		};
		print_lines( output );
	}
}