#pragma once

#include "Arduino.h"
#include "ArduinoBLE.h"

class BleRemoteDevice_interface{
public:
	virtual String getLocalName() = 0;
	virtual String getServiceUuid() = 0;
	virtual String getCharacteristicUuid() = 0;
	BLEDevice bleDevice;
	BLECharacteristic bleCharacteristic;
};
