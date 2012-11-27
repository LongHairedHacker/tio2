#ifndef GPIO_HPP_
#define GPIO_HPP_ GPIO_HPP_

#include <inttypes.h>

namespace tio2 {

struct GPIO {
	private:
		volatile uint32_t _values[256];
		volatile uint32_t _direction;
		volatile uint32_t _interrupt_sense;
		volatile uint32_t _interrupt_both_edges;
		volatile uint32_t _interrupt_event;
		volatile uint32_t _interrupt_mask;
		volatile uint32_t _raw_interrupt_status;
		volatile uint32_t _masked_interrupt_status;
		volatile uint32_t _interrupt_clear;
		volatile uint32_t _alternate_function_select;
		volatile uint8_t rsvd_1[0x500 - 0x420 - 4];
		volatile uint32_t _drive_select_2ma;
		volatile uint32_t _drive_select_4ma;
		volatile uint32_t _drive_select_8ma;
		volatile uint32_t _open_drain_select;
		volatile uint32_t _pullup_select;
		volatile uint32_t _pulldown_select;
		volatile uint32_t _slew_rate_control_select;
		volatile uint32_t _digital_enable;
		volatile uint32_t _lock;
		volatile uint32_t _commit;
		volatile uint32_t _analog_mode_select;
		volatile uint32_t _port_control;
		volatile uint32_t _adc_control;
		volatile uint32_t _dma_control;
		volatile uint8_t rsvd_2[0x1000 - 0x534 - 4];

	public:
		GPIO() = delete;
		GPIO(GPIO&) = delete;

		enum class InterruptMode {
			None,
			RisingEdge,
			FallingEdge,
			BothEdges,
			HighLevel,
			LowLevel
		};

		uint32_t operator[](uint8_t mask) const { return _values[mask]; }
		volatile uint32_t& operator[](uint8_t mask) { return _values[mask]; }

		uint32_t outputs() const { return _direction; }
		volatile uint32_t& outputs() { return _direction; }

		// Set interrupt mode of the given pin.
		// Any interrupt status that has been set for the pins will be cleared.
		void interruptMode(uint8_t pin, InterruptMode mode)
		{
			pin &= 7;

			_interrupt_mask &= ~(1 << pin);
			if (mode != InterruptMode::None) {
				switch (mode) {
					case InterruptMode::RisingEdge:
						_interrupt_sense &= ~(1 << pin);
						_interrupt_both_edges &= ~(1 << pin);
						_interrupt_event |= (1 << pin);
						break;

					case InterruptMode::FallingEdge:
						_interrupt_sense &= ~(1 << pin);
						_interrupt_both_edges &= ~(1 << pin);
						_interrupt_event &= ~(1 << pin);
						break;

					case InterruptMode::BothEdges:
						_interrupt_sense &= ~(1 << pin);
						_interrupt_both_edges |= (1 << pin);
						break;

					case InterruptMode::HighLevel:
						_interrupt_sense |= (1 << pin);
						_interrupt_event |= (1 << pin);
						break;

					case InterruptMode::LowLevel:
						_interrupt_sense |= (1 << pin);
						_interrupt_event &= ~(1 << pin);
						break;

					default:
						// shut up the fucking compiler
						break;
				}

				_interrupt_clear = (1 << pin);
				_interrupt_mask |= (1 << pin);
			}
		}

		InterruptMode interruptMode(uint8_t pin) const
		{
			pin &= 7;

			if (_interrupt_mask & (1 << pin)) {
				if (_interrupt_sense & (1 << pin)) {
					return _interrupt_event & (1 << pin) ? InterruptMode::HighLevel : InterruptMode::LowLevel;
				} else {
					if (_interrupt_both_edges & (1 << pin)) {
						return InterruptMode::BothEdges;
					} else {
						return _interrupt_event & (1 << pin) ? InterruptMode::RisingEdge : InterruptMode::FallingEdge;
					}
				}
			} else {
				return InterruptMode::None;
			}
		}

		uint32_t interrupt_sense() const { return _interrupt_sense; }
		volatile uint32_t& interrupt_sense() { return _interrupt_sense; }

		uint32_t interrupt_both_edges() const { return _interrupt_both_edges; }
		volatile uint32_t& interrupt_both_edges() { return _interrupt_both_edges; }

		uint32_t interrupt_event() const { return _interrupt_event; }
		volatile uint32_t& interrupt_event() { return _interrupt_event; }

		uint32_t interrupt_mask() const { return _interrupt_mask; }
		volatile uint32_t& interrupt_mask() { return _interrupt_mask; }

		uint32_t raw_interrupt_status() const { return _raw_interrupt_status; }

		uint32_t masked_interrupt_status() const { return _masked_interrupt_status; }

		void clear_interrupt_status(uint8_t pins) { _interrupt_clear = pins; }

		// TODO: alternate function select

		enum class Drive {
			Strength2mA,
			Strength4mA,
			Strength8mA,
			Strength8mA_SlewRateLimited
		};

		void drive_strength(uint8_t pin, Drive drive)
		{
			switch (drive) {
				case Drive::Strength2mA:
					_drive_select_2ma = (1 << (pin & 7));
					return;

				case Drive::Strength4mA:
					_drive_select_4ma = (1 << (pin & 7));
					return;

				case Drive::Strength8mA:
					_drive_select_8ma = (1 << (pin & 7));
					return;

				case Drive::Strength8mA_SlewRateLimited:
					_drive_select_8ma = (1 << (pin & 7));
					_slew_rate_control_select = (1 << (pin & 7));
					return;
			}
		}

		Drive drive_strength(uint8_t pin) const
		{
			if (_drive_select_2ma & (1 << (pin & 7))) {
				return Drive::Strength2mA;
			} else if (_drive_select_4ma & (1 << (pin & 7))) {
				return Drive::Strength4mA;
			} else if (_drive_select_8ma & (1 << (pin & 7))) {
				return _slew_rate_control_select & (1 << (pin & 7)) ? Drive::Strength8mA_SlewRateLimited : Drive::Strength8mA;
			}
		}

		uint32_t open_drain_on() const { return _open_drain_select; }
		volatile uint32_t& open_drain_on() { return _open_drain_select; }

		uint32_t pullup_on() const { return _pullup_select; }
		volatile uint32_t& pullup_on() { return _pullup_select; }

		uint32_t pulldown_on() const { return _pulldown_select; }
		volatile uint32_t& pulldown_on() { return _pulldown_select; }

		uint32_t digital_enable() const { return _digital_enable; }
		volatile uint32_t& digital_enable() { return _digital_enable; }

		uint32_t lock() const { return _lock; }
		volatile uint32_t& lock() { return _lock; }

		uint32_t commit() const { return _commit; }
		volatile uint32_t& commit() { return _commit; }

		// TODO: analog mode select
		// TODO: port control
		// TODO: adc control
		// TODO: dma control

		enum class Port {
			A = 1,
			B = 2,
			C = 4,
			D = 8,
			E = 16,
			F = 32
		};
};

static_assert(__is_standard_layout(GPIO), "Damnit!");
static_assert(sizeof(GPIO) == 0x1000, "Damnit!");

class GPIOControlRegister {
	protected:
		volatile uint32_t _r;

		uint32_t make_mask()
		{
			return 0;
		}

		template<class... Ports>
		uint32_t make_mask(GPIO::Port port, Ports... rest)
		{
			return static_cast<uint32_t>(port) | make_mask(rest...);
		}

	public:
		GPIOControlRegister() = delete;
		GPIOControlRegister(GPIOControlRegister&) = delete;
};

class GPIOPortFlags : GPIOControlRegister {
	public:
		template<class... Ports>
		void enable(Ports... ports) { _r |= make_mask(ports...); }

		template<class... Ports>
		void disable(Ports... ports) { _r &= ~make_mask(ports...); }

		template<class... Ports>
		void set(Ports... ports) { _r = make_mask(ports...); }

		template<class... Ports>
		bool enabled(Ports... ports) { return (_r & make_mask(ports...)) == make_mask(ports...); }
};

static_assert(__is_standard_layout(GPIOPortFlags), "Damnit!");
static_assert(sizeof(GPIOPortFlags) == 4, "Damnit!");

class GPIOPortStatus : GPIOControlRegister {
	public:
		template<class... Ports>
		bool ready(Ports... ports) { return (_r & make_mask(ports...)) == make_mask(ports...); }
};

static_assert(__is_standard_layout(GPIOPortStatus), "Damnit!");
static_assert(sizeof(GPIOPortStatus) == 4, "Damnit!");

// Linker magic moves these to the appropriate locations in IO space
extern GPIO gpio_a;
extern GPIO gpio_b;
extern GPIO gpio_c;
extern GPIO gpio_d;
extern GPIO gpio_e;
extern GPIO gpio_f;

extern GPIOPortFlags gpio_reset_state;
extern GPIOPortFlags gpio_in_run;
extern GPIOPortFlags gpio_in_sleep;
extern GPIOPortFlags gpio_in_deep_sleep;
extern GPIOPortStatus gpio_status;

}

#endif
