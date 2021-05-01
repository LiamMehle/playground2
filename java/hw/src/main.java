public class main {


	private static void puts( String s ) {
		System.out.println( s );
	}

	public static void main( String[] args ) {
		puts("Hello, world");


		String array[] = {"hello, ", "world", " :)"};
		for( int i = 0; i < array.length; i++ )
			puts(array[i]);
	}
}
