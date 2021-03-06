#pragma once

#include <stdint.h>

namespace irRemoteRaw{
	struct Configuration{
		void (*infraRed_on)();
		void (*infraRed_off)();
		void (*waitCarrierHalfPeriod)();
	};

	extern void (*sendCarrierNTimes)(uint16_t periods, Configuration configuration);
}
