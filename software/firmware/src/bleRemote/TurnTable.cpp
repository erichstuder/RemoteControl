#include "TurnTable.h"
#include <Arduino.h>
#include <ArduinoBLE.h>

String TurnTable::getLocalName(){
	return "TurnTable";
}

String TurnTable::getServiceUuid(){
	return "19b10000-e8f2-537e-4f6c-d104768a1214";
}

String TurnTable::getCharacteristicUuid(){
	return "19b10001-e8f2-537e-4f6c-d104768a1214";
}

void TurnTable::send(Command command){
	if(!isConnected()){
		return;
	}

	switch(command){
		case Command::TurnClockwise:
			bleCharacteristic.writeValue((byte)0x01);
			break;
		case Command::TurnCounterClockwise:
			bleCharacteristic.writeValue((byte)0x02);
			break;
		default:
			//do nothing
			break;
	}
}

/*
namespace turnTable{
	enum class State{
		StartScanning,
		Scanning,
		Connected,
	};

	static State state;
	static BLEDevice peripheral;
	static BLECharacteristic turnCharacteristic;

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

	static void sendCommand_Implementation(Command command){
		switch(command){
			case Command::TurnClockWise:
				turnCharacteristic.writeValue((byte)0x01);
				break;
			case Command::TurnCounterClockWise:
				turnCharacteristic.writeValue((byte)0x02);
				break;
			default:
				//do nothing
				break;
		}
	}
	void (*sendCommand)(Command command) = sendCommand_Implementation;

	static bool isConnected_Implementation(){
		return state == State::Connected;
	}
	bool (*isConnected)() = isConnected_Implementation;

	void disconnect_Implementation(){
		peripheral.disconnect();
	}
	void (*disconnect)() = disconnect_Implementation;
}
*/
