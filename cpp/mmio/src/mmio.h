#ifndef MMIO_H
#define MMIO_H

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
				constexpr uint64_t offset_gpio_lev     = 0x0000'0034;
			constexpr uint64_t offset_interrupt      = 0x0000'B000;
				constexpr uint64_t offset_irq_pending  = 0x0000'0200;
				constexpr uint64_t offset_irq_pending_1 = 0x000'0204;
				constexpr uint64_t offset_irq_pending_2 = 0x000'0208;
				constexpr uint64_t offset_fiq_enable   = 0x0000'020C;
				constexpr uint64_t offset_irq_enable_1  = 0x000'0210;
				constexpr uint64_t offset_irq_enable_2  = 0x000'0214;
				constexpr uint64_t offset_irq_enable   = 0x0000'0218;
				constexpr uint64_t offset_irq_disable_1 = 0x000'021C;
				constexpr uint64_t offset_irq_disable_2 = 0x000'0220;
				constexpr uint64_t offset_irq_disable   = 0x000'0224;


		template<typename T1, typename T2>
		constexpr
		T1* offset( T1* addr, T2 offset ) { // offset by *offset* bytes instead
			return reinterpret_cast<T1*>((reinterpret_cast<char*>(addr) + offset));
		}
		[[nodiscard]]
		constexpr
		uint32_t* get_gpio_read_address( uint32_t* base_io ) noexcept {
			return offset( base_io, offset_gpio + offset_gpio_lev );
		}
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
	[[nodiscard]]
	bool gpio_read( uint32_t* base_io, int pin ) noexcept ;
	[[nodiscard]]
	volatile uint32_t gpio_read_page( uint32_t* base_io ) noexcept ;
}

#endif
