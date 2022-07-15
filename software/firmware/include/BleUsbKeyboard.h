#pragma once

#include "BleAccessory.h"

class BleUsbKeyboard : public BleAccessory{
public:
	String getLocalName();
	String getServiceUuid();
	String getCharacteristicUuid();
	void sendText(String text);
};
