#ifndef MMIOH
#define MMIOH

#include <sys/types.h>
#include <cstdint>

namespace peripheral {

	namespace address {
		constexpr uint64_t io_phys = 0x3F00'0000; //ends in 0x3FFF'FFFF
		constexpr uint64_t io_bus  = 0x7E00'0000; //ends in 0x7EFF'FFFF

		constexpr uint64_t offset_gpio            = 0x0020'0000;
		constexpr uint64_t offset_gpio_mode       = 0x0;
		constexpr uint_fast8_t function_sel_size  = 0x4; // there's 6 of them
		constexpr uint64_t offset_gpio_output_set = 0x1C;
		constexpr uint64_t offset_gpio_output_clr = 0x28;
		constexpr uint64_t offset_gpio_input      = 0x34;

		#pragma pack()
		struct offset_gpio_mode_s {
			struct mode { char value:3; } fsel[10];
		};
	}


	constexpr
	const char* bus_to_phys( const char* addr ) {
		return addr-(address::io_bus-address::io_phys);
	}

	constexpr
	const char* phys_to_bus( const char* addr ) {
		return addr+(address::io_bus-address::io_phys);
	}

	[[nodiscard]] // logical error for a mapped page to be forgotten
	void* map( uint64_t address, const size_t length ) noexcept;// nullptr on fail
	int unmap( const void* address, const size_t length ) noexcept; // -1  on fail
	// used for correcting unlikely errors
	int decrement_refc() noexcept;
	// misc features
	void fill_recognisable( char* const addr, const size_t size ) noexcept;
}

#endif
