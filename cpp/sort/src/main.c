#include <assert.h>

void swap( int* arr, int a, int b ) {
	const int temp = arr[b];
	arr[b] = arr[a]; // b < a
	arr[a] = temp;   // a < old b
}

int partition( int* arr, int len ) {
	const int pivot_index = 0;
	const int pivot_value = arr[pivot_index];

	// if pivot isn't the 0th element, move it there

	int lo_index = 1; // do not process pivot
	int hi_index = len-1;

	while(1) {
		// find next value bigger than pivot_value from the bottom
		while( lo_index != hi_index ) {
			if ( arr[lo_index] > pivot_value ) { // if found, continue
				break;
			}
			lo_index++;
		}
		if ( lo_index == hi_index )
			goto done;

		while( lo_index != hi_index ) {
			if ( arr[hi_index] > pivot_value ) { // if found, continue
				break;
			}
			hi_index--;
		}
		if ( lo_index == hi_index )
			goto done;

		// if found both values and not the same index
		swap( arr, lo_index, hi_index );
	}
	done:
	// both indecies should be pointing at the same element and in bounds
	assert( lo_index == hi_index );
	assert( lo_index < len );
	assert( lo_index > 0 );
	assert( hi_index < len );
	assert( hi_index > 0 );

	int pivot_index_new; // new position of pivot

	if( arr[lo_index] < pivot_value )
		// pointed to value should be on the left of pivot
		pivot_index_new = lo_index;
	else
		// pointed to value should be on the right of pivot
		pivot_index_new = lo_index-1;

	swap( arr, pivot_index, pivot_index_new ); // move pivot into position
	return pivot_index_new;
}

void quick_sort( int* arr, int len ) {
	__attribute__((unused))
	const int pivot_index = partition( arr, len );
}

int main() {
	int array[] = { 2, 4, 3, 1, 2, 6 };
	const int array_size = sizeof( array ) / sizeof( array[0] );
	quick_sort( array, array_size );
	return 0;
}
