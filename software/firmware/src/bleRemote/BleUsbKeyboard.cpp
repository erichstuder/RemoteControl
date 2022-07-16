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

void BleUsbKeyboard::sendText(String text){
	if(!isConnected()){
		return;
	}
	bleCharacteristic.writeValue(text.c_str());
}
