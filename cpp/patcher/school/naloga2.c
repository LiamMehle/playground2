
enum state {
	side_green,
	side_green,
	side_closed, // switching to main_yellow
	main_closed, // switching to side_yellow
	main_yellow,
	main_yellow
}

int main_2() {
	int state = 0;
	for(;;){
		switch(state) {
			case main_closed:
				wait(interval[0]);
				state = side_yellow;
				break;
			case side_yellow:
				wait(interval[1]);
				state = side_green;
				break;
			case side_green:
				wait(interval[2]);
				state = side_closed;
				break;

			case side_closed:
				wait(interval[0]);
				state = main_yellow;
				break;
			case main_yellow:
				wait(interval[1]);
				state = main_green;
				break;
			case main_green:
				wait(interval[2]);
				if(side_waiting())
					state = main_closed;
				break;			
	
		}
	}
	return 0;
}