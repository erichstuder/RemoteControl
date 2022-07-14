#include "BleUsbKeyboard.h"
#include <Arduino.h>
#include <ArduinoBLE.h>

String BleUsbKeyboard::getLocalName(){
	return "BLE_to_USB-Keyboard";
}

String BleUsbKeyboard::getServiceUuid(){
	return "19b10000-e8f2-537e-4f6c-d104768a1214";
}

String BleUsbKeyboard::getCharacteristicUuid(){
	return "19b10001-e8f2-537e-4f6c-d104768a1214";
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
