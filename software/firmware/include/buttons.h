#pragma once

#include <Arduino.h>

namespace buttons{
	enum class ButtonId{
		SW1, SW2, SW3, SW4, SW5,
		SW6, SW7, SW8, SW9, SW10,
		SW11, SW12, SW13, SW14, SW15, None
	} ;

	extern void (*init)();
	extern void (*tick)();
	extern ButtonId (*getPressedEvent)();
	extern void (*clearPressedEvent)();
	bool isPressed(ButtonId id);
	extern unsigned long (*getLastPressedMillis)();
}
