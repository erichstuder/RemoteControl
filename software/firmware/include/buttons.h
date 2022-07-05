#pragma once

namespace buttons{
	enum class Buttons{
		SW1,
		SW3,
		SW4,
		SW6,
		None,
	};

	extern void (*init)();
	extern void (*tick)();
	extern Buttons (*getPressedEvent)();
	extern void (*clearPressedEvent)();
	extern bool (*sw4_pressed)();
	extern bool (*sw6_pressed)();
	extern unsigned long (*getLastPressedMillis)();
}
