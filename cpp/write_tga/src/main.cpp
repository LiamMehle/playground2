#include <iostream>
#include <fstream>
#include <alloca.h>
#include <array>
#define BYTE_RANGE (1<<8)

////// targa file header

typedef struct {
	char id_length;      // length of id field (number of bytes - max 255)
	char map_type;       // colormap field (0 or 1; no map or 256 entry palette)
	char image_type;     // ( 0 - no image data included
			     //	  1 - uncompressed, color mapped image
			     //	  2 - uncompressed, RGB image
		 	     //	  3 - uncompressed, black & white image
			     //	  9 - run-length encoded(RLE-lossless compression),color mapped image
			     //	 10 - RLE, RGB image
			     //	 11 - compressed, black & white image )

	int map_first;       // first entry index for color map
	int map_length;      // total number of entries in color map
	char map_entry_size; // number of bits per entry

	int x;               // x cooridinate of origin
	int y;               // y cooridinate of origin

	int width;           // width in pixels
	int height;          // height in pixels

	char bits_per_pixel; // number of bits per pixel
	
	char misc;           // srsly? "scan origin and alpha bits" this example uses scan origin
			     // honestly, don't know what's going on here. we pass in a hex value
			     // :shrug_emoji:		
} targa_header;

template<typename T>
constexpr
char lsbyte (T number) {
	return static_cast<char>(number % BYTE_RANGE);
}

template<typename T>
constexpr
char msbyte (T number) {
	return static_cast<char>(number / BYTE_RANGE);
}

////// write header function

void write_header (const targa_header& header, std::fstream& tga) {
	const char buffer[] = {
		header.id_length,
		header.map_type,
		header.image_type,

		lsbyte(header.map_first),
		msbyte(header.map_first),  
    
		lsbyte(header.map_length),
		msbyte(header.map_length),      

		header.map_entry_size,

		lsbyte(header.x),
		msbyte(header.x),      
		lsbyte(header.y),
		msbyte(header.y),      

		lsbyte(header.width),
		msbyte(header.width),      
		lsbyte(header.height),
		msbyte(header.height),      

		header.bits_per_pixel,
		header.misc
	};
	tga.write(buffer, sizeof(buffer));
}

////// MAIN

int main () {
	// set header values

	constexpr targa_header header = {
		.id_length = 0,
		.map_type = 0,
		.image_type = 2,     // uncompressed RGB image

		.map_first = 0,
		.map_length = 0,
		.map_entry_size = 0,

		.x = 0,
		.y = 0,
		.width = 500,
		.height = 500,

		.bits_per_pixel = 8*3,
		.misc = 0x20        // scan from upper left corner, wut dude
	};
	// start to write file
	
	std::fstream tga;
	tga.open("result.tga", std::ios::out | std::ios::binary); // overwrite

	write_header (header, tga);


	{
		constexpr std::array rgb = {
					static_cast<char>(170),
					static_cast<char>(200),
					static_cast<char>(20)
		};

		for (int y = 0; y < header.height; y++)
			for (int x = 0; x < header.width; x++) {
				// B G R order 
				
				tga.write(rgb.data(), 3);
			}
	}

	return 0;
}