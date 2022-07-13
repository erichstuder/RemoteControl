#pragma once

#include "Arduino.h"
#include "ArduinoBLE.h"

class IBleRemoteDevice{
public:
	virtual String getLocalName() = 0;
	virtual String getServiceUuid() = 0;
	virtual String getCharacteristicUuid() = 0;
	BLEDevice bleDevice;
	BLECharacteristic bleCharacteristic;
};
