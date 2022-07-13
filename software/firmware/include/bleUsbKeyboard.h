#pragma once

#include "BleRemoteDevice_interface.h"

class BleUsbKeyboard : public BleRemoteDevice_interface{
public:
	String getLocalName();
	String getServiceUuid();
	String getCharacteristicUuid();
	/*void sendText(String text);
	bool isConnected();
	void disconnect();*/
};
