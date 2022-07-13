#pragma once

#include "IBleRemoteDevice.h"

class BleUsbKeyboard : public IBleRemoteDevice{
public:
	String getLocalName();
	String getServiceUuid();
	String getCharacteristicUuid();
	/*void sendText(String text);
	bool isConnected();
	void disconnect();*/
};
