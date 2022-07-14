#pragma once

//#include <Arduino.h>
#include "BleUsbKeyboard.h"
#include "BleAccessory.h"

class KodiRemote : public BleUsbKeyboard{
public:
	enum class Command{
		Up,
	};

	void send(Command command);

private:
	using BleUsbKeyboard::sendText;
};
