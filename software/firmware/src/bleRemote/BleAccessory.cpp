#include "BleAccessory.h"

void BleAccessory::setBleDevice(BLEDevice bleDevice){
	this->bleDevice = bleDevice;
}

bool BleAccessory::connect(){
	if(!bleDevice){
		return false;
	}

	if(bleDevice.connected()){
		return true;
	}

	if(!bleDevice.connect()){
		return false;
	}
	
	if(!bleDevice.discoverAttributes()){
		bleDevice.disconnect();
		return false;
	}

	BLECharacteristic characteristic = bleDevice.characteristic(getCharacteristicUuid().c_str());

	if(!characteristic){
		bleDevice.disconnect();
		return false;
	}

	if(!characteristic.canWrite()){
		bleDevice.disconnect();
		return false;
	}

	bleCharacteristic = characteristic;

	return true;
}

void BleAccessory::disconnect(){
	if(!bleDevice){
		return;
	}
	bleDevice.disconnect();
}

bool BleAccessory::isConnected(){
	if(!bleDevice){
		return false;
	}
	return bleDevice.connected();
}
