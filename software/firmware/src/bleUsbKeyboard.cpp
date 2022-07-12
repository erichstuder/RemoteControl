#include "bleUsbKeyboard.h"
#include <Arduino.h>
#include <ArduinoBLE.h>

namespace bleUsbKeyboard{
	enum class State{
		StartScanning,
		Scanning,
		Connected,
	};

	static State state;
	static BLEDevice peripheral;
	static BLECharacteristic textCharacteristic;

	static bool connectToPeripheral();

	static void init_Implementation(){
		BLE.begin();
		state = State::StartScanning;
	}
	void (*init)() = init_Implementation;

	static void tick_Implementation(){
		const char* UUID = "19b10000-e8f2-537e-4f6c-d104768a1214";

		switch(state){
			case State::StartScanning:
				BLE.scanForUuid(UUID, true);
				state = State::Scanning;
				break;
			case State::Scanning:
				peripheral = BLE.available();
				if(peripheral && peripheral.localName() == "BLE_to_USB-Keyboard"){
					BLE.stopScan();
					if(connectToPeripheral()){
						state = State::Connected;
					}
					else{
						state = State::StartScanning;
					}
				}
				break;
			case State::Connected:
				if(!peripheral.connected()){
					state = State::StartScanning;
				}
				break;
			default:
				state = State::StartScanning;
				break;
		}
	}
	void (*tick)() = tick_Implementation;

	static bool connectToPeripheral(){
		if(!peripheral.connect()){
			return false;
		}
		
		if(!peripheral.discoverAttributes()){
			peripheral.disconnect();
			return false;
		}

		textCharacteristic = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1214");

		if(!textCharacteristic){
			peripheral.disconnect();
			return false;
		}

		if(!textCharacteristic.canWrite()){
			peripheral.disconnect();
			return false;
		}

		return true;
	}

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
}
