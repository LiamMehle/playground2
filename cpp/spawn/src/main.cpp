#include <iostream>
#include <fstream>
#include <cstring>
#include <linux/input.h>

using namespace std;

//structure to hold event info
union buffer_t { struct input_event ev; char data[sizeof(input_event)]; };
buffer_t buffer;
int main() {
	try {

	auto const kb_path = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";
	ifstream file(kb_path);

	//check if file opened!
	if(!file.is_open()) {
		throw (char*)"cannot open keyboard file";
	}

	while(true) {
		file.read(buffer.data, sizeof(buffer_t));
		//now lets read the event
		//event type
		if(buffer.ev.type == EV_KEY) {
			if(buffer.ev.code == KEY_ESC) {
				
				cout << "The Escape Key Was Pushed!" << endl;
				break;
			}
			else {
				
				cout << "Key Press " << buffer.ev.code << endl;
			}
		}
	}
	} catch (char* e) {
		std::cout << "[error]: " << e << '\n';
	}
	return 0;
}