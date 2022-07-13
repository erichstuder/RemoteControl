#include "BleUsbKeyboard.h"
#include <Arduino.h>
#include <ArduinoBLE.h>

String BleUsbKeyboard::getLocalName(){
	//bleDevice.
	return "";
}

String BleUsbKeyboard::getServiceUuid(){
	return"";
}

String BleUsbKeyboard::getCharacteristicUuid(){
	return "";
}


	/*virtual String getServiceUuid() = 0;
	virtual String getCharacteristicUuid() = 0;*/

/*
namespace bleUsbKeyboard{
	enum class State{
		StartScanning,
		Scanning,
		Connected,
	};

	static BLEDevice bleDevice;
	static BLECharacteristic textCharacteristic;



	static void sendText_Implementation(String text){
		textCharacteristic.writeValue(text.c_str());
	}
	void (*sendText)(String text) = sendText_Implementation;

	static bool isConnected_Implementation(){
		return state == State::Connected;
	}
	bool (*isConnected)() = isConnected_Implementation;

	void disconnect_Implementation(){
		peripheral.disconnect();
	}
	void (*disconnect)() = disconnect_Implementation;
}*/
