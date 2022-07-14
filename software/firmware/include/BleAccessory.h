#pragma once

#include "Arduino.h"
#include "ArduinoBLE.h"

class BleAccessory{
public:
	virtual String getLocalName() = 0;
	virtual String getServiceUuid() = 0;
	virtual String getCharacteristicUuid() = 0;
	void setBleDevice(BLEDevice bleDevice);
	bool connect();
	void disconnect();
	bool isConnected();
protected:
	BLEDevice bleDevice;
	BLECharacteristic bleCharacteristic;
};
