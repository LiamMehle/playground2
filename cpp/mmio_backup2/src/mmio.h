#ifndef MMIOH
#define MMIOH

#include <cstdint>

namespace peripheral {

	namespace address {
		constexpr uint64_t io_size /*<--------->*/ = 0x00FF'FFFF;
		constexpr uint64_t io_phys /*<--------->*/ = 0x3F00'0000; //ends in 0x3FFF'FFFF
		constexpr uint64_t io_bus  /*<--------->*/ = 0x7E00'0000; //ends in 0x7EFF'FFFF
			constexpr uint64_t offset_gpio           = 0x0020'0000;
				constexpr uint64_t offset_gpio_fs      = 0x0000'0000;
				constexpr uint64_t offset_gpio_out_set = 0x0000'001C;
				constexpr uint64_t offset_gpio_out_clr = 0x0000'0028;
				constexpr uint64_t offset_gpio_in      = 0x0000'0034;
	}

	enum Function:char {
		in   = 0,
		out  = 1,
		alt0 = 4,
		alt1 = 5,
		alt2 = 6,
		alt3 = 7,
		alt4 = 3,
		alt5 = 2
	};
	enum Level:bool {
		low = false,
		high = true
	};

	constexpr
	const uint32_t* bus_to_phys( const uint32_t* addr ) noexcept {
		return addr-(address::io_bus-address::io_phys);
	}

	constexpr
	const uint32_t* phys_to_bus( const uint32_t* addr ) noexcept {
		return addr+(address::io_bus-address::io_phys);
	}

	[[nodiscard]] // logical error for a mapped page to be forgotten
	uint32_t* map_io() noexcept; // nullptr on fail
	int unmap_io( uint32_t* addr ) noexcept; // -1  on fail
	// used for correcting unlikely errors
	int decrement_refc() noexcept;
	// misc features
	void fill_recognisable( uint32_t* const addr, const uint64_t size ) noexcept;
	void gpio_fs( uint32_t* base_io, int pin, int function ) noexcept ; // sets pin function
	void gpio_write( uint32_t* base_io, int pin, bool level ) noexcept ; // sets pin level
}

#endif
