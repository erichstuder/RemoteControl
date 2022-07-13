//#include "bleRemoteHandler.h"
#include <Arduino.h>
#include <ArduinoBLE.h>

//TODO: testen, ob man nach mehreren Service UUIDs gleichzeitig scannen kann.
//TODO: testen, ob man nicht auch nach einem "Gerät" statt nach einem Service scannen kann.
//TODO: für die einzelnen Geräte ein Interface erstellen z.B. bleRemoteDevice_interface.cpp

/*
namespace bleRemoteHandler{
	enum class State{
		StartScanning,
		Scanning,
		Connected,
	};

	static State state;

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
				if(peripheral && peripheral.localName() == "TurnTable"){
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

		turnCharacteristic = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1214");

		if(!turnCharacteristic){
			peripheral.disconnect();
			return false;
		}

		if(!turnCharacteristic.canWrite()){
			peripheral.disconnect();
			return false;
		}

		return true;
	}
}
*/
