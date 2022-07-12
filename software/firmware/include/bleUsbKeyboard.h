#pragma once

#include <Arduino.h>

namespace bleUsbKeyboard{
	extern void (*init)();
	extern void (*tick)();
	extern void (*sendText)(String text);
	extern bool (*isConnected)();
	extern void (*disconnect)();
}