#ifndef CLOCK_HPP_
#define CLOCK_HPP_ CLOCK_HPP_

#include <inttypes.h>

namespace tio2 {

enum class XTAL {
	X_4_MHz = 0x06,
	X_4_096_Mhz = 0x07,
	X_4_9152_Mhz = 0x08,
	X_5_Mhz = 0x09,
	X_5_12_MHz = 0x0a,
	X_6_MHz = 0x0b,
	X_6_144_MHz = 0x0c,
	X_7_3728_MHz = 0x0d,
	X_8_MHz = 0x0e,
	X_8_192_MHz = 0x0f,
	X_10_MHz = 0x10,
	X_12_MHz = 0x11,
	X_12_288_MHz = 0x12,
	X_13_56_MHz = 0x13,
	X_14_31818_MHz = 0x14,
	X_16_MHz = 0x15,
	X_16_384_MHz = 0x16,
	X_18_MHz = 0x17,
	X_20_MHz = 0x18,
	X_24_MHz = 0x19,
	X_25_MHz = 0x1a
};

enum class OscSource {
	Main = 0,
	Internal = 1,
	InternalDiv4 = 2,
	Internal_30_kHz = 3,
	External_32_768_kHz = 7
};

class clock_config {
	private:
		uint32_t rcc;
		uint32_t rcc2;

		static constexpr uint32_t ACG = 1 << 27;
		static constexpr uint32_t USE_SYSDIV = 1 << 22;
		static constexpr uint32_t XTAL_MASK = 1 << 6;
		static constexpr uint32_t XTAL_SHIFT = 6;
		static constexpr uint32_t IOSCDIS = 1 << 1;
		static constexpr uint32_t MOSCDIS = 1 << 0;

		static constexpr uint32_t USE_RCC2 = 1 << 31;
		static constexpr uint32_t DIV_400 = 1 << 30;
		static constexpr uint32_t SYSDIV_MASK = 0x7f << 22;
		static constexpr uint32_t SYSDIV_SHIFT = 22;
		static constexpr uint32_t USB_PLL_PWRDN = 1 << 14;
		static constexpr uint32_t PLL_PWRDWN = 1 << 13;
		static constexpr uint32_t PLL_BYPASS = 1 << 11;
		static constexpr uint32_t OSCSRC_MASK = 0x7 << 4;
		static constexpr uint32_t OSCSRC_SHIFT = 4;

		clock_config set_rcc(uint32_t rcc) const
		{
			return clock_config(rcc, rcc2);
		}

		clock_config set_rcc2(uint32_t rcc2) const
		{
			return clock_config(rcc, rcc2);
		}

		clock_config set_both(uint32_t rcc, uint32_t rcc2) const
		{
			return clock_config(rcc, rcc2);
		}

	public:
		clock_config(uint32_t rcc, uint32_t rcc2) : 
			rcc(rcc),
			rcc2(rcc2 | USE_RCC2 | DIV_400)
		{
		}

		clock_config() :
			rcc(0),
			rcc2(USE_RCC2 | DIV_400)
		{
		}

		uint32_t rcc_val() const { return rcc; }
		uint32_t rcc2_val() const { return rcc2; }

		clock_config auto_clock_gate(bool b) const
		{
			return set_rcc(b ? rcc | ACG : rcc & ~ACG);
		}

		bool auto_clock_gate() const { return rcc & ACG; }

		clock_config sysdiv(uint32_t div) const
		{
			div &= 0x7f;
			return set_both(
					(rcc & ~USE_SYSDIV) | (div > 1 ? USE_SYSDIV : 0),
					(rcc2 & ~SYSDIV_MASK & ~USE_SYSDIV)
						| (div > 1 ? USE_SYSDIV | (div << SYSDIV_SHIFT) : 0));
		}

		uint32_t sysdiv() const { return (rcc2 & SYSDIV_MASK) >> SYSDIV_SHIFT; }

		clock_config pll_powerdown(bool b) const
		{
			return set_rcc2(b ? rcc2 | PLL_PWRDWN : rcc2 & ~PLL_PWRDWN);
		}

		bool pll_powerdown() const { return rcc2 & PLL_PWRDWN; }

		clock_config pll_bypass(bool b) const
		{
			return set_rcc2(b ? rcc2 | PLL_BYPASS : rcc2 & ~PLL_BYPASS);
		}

		bool pll_bypass() const { return rcc2 & PLL_BYPASS; }

		clock_config xtal(XTAL x) const
		{
			return set_rcc((rcc & ~XTAL_MASK) | (static_cast<uint32_t>(x) << XTAL_SHIFT));
		}

		XTAL xtal() const { return static_cast<XTAL>((rcc2 & XTAL_MASK) >> XTAL_SHIFT); }

		clock_config internal_oscillator_disable(bool b) const
		{
			return set_rcc(b ? rcc | IOSCDIS : rcc & ~IOSCDIS);
		}

		bool internal_oscillator_disable() const { return rcc & IOSCDIS; }

		clock_config main_oscillator_disable(bool b) const
		{
			return set_rcc(b ? rcc | MOSCDIS : rcc & ~MOSCDIS);
		}

		bool main_oscillator_disable() const { return rcc & MOSCDIS; }

		clock_config oscillator_source(OscSource src) const
		{
			return set_rcc2((rcc2 & ~OSCSRC_MASK) | (static_cast<uint32_t>(src) << OSCSRC_SHIFT));
		}

		OscSource oscillator_source() const { return static_cast<OscSource>((rcc2 & OSCSRC_MASK) >> OSCSRC_SHIFT); }
};

class CLOCK {
	private:
		volatile uint8_t rsvd_1[0x60];
		volatile uint32_t rcc;
		volatile uint8_t rsvd_2[0x70 - 0x60 - 4];
		volatile uint32_t rcc2;
		volatile uint8_t rsvd_3[0x168 - 0x70 - 4];
		volatile uint32_t pllstat;

	public:
		CLOCK() = delete;
		CLOCK(CLOCK&) = delete;

		clock_config config() const
		{
			return clock_config(rcc, rcc2);
		}

		void config(const clock_config& cfg)
		{
			if (cfg.oscillator_source() == OscSource::Main
					&& !cfg.main_oscillator_disable()
					&& !cfg.pll_powerdown()
					&& !cfg.pll_bypass()) {
				config(cfg.pll_bypass(true));
			}
			rcc = cfg.rcc_val();
			rcc2 = cfg.rcc2_val();
		}
};

static_assert(__is_standard_layout(CLOCK), "Damnit!");
static_assert(sizeof(CLOCK) == 0x16C, "Damnit!");

extern CLOCK clock;

}

#endif
