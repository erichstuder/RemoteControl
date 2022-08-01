#include <Arduino.h>
#include <ArduinoBLE.h>
#include "bleRemoteHandler.h"
#include "BleAccessory.h"
#include "TurnTable.h"
#include "KodiRemote.h"

//TODO: evtl. testen, ob man nach mehreren Service UUIDs gleichzeitig scannen kann.
//TODO: für die einzelnen Geräte ein Interface erstellen z.B. bleRemoteDevice_interface.cpp

namespace bleRemoteHandler{
	static TurnTable turnTable;
	static KodiRemote kodiRemote;
	static std::array<BleAccessory*, 2> accessories = {&turnTable, &kodiRemote};

	enum class State{
		StartScanning,
		Scanning,
		AllConnected,
	};

	static State state;

	static bool allAccessoriesConnected();

	static void init_Implementation(){
		BLE.begin();
		state = State::StartScanning;
	}
	void (*init)() = init_Implementation;

	static void tick_Implementation(){
		switch(state){
			case State::StartScanning:
				BLE.scan(true);
				state = State::Scanning;
				break;
			case State::Scanning:{
				BLEDevice bleDevice = BLE.available();
				if(bleDevice){
					for(auto accessory : accessories){
						if(!accessory->isConnected()){
							bool isName = bleDevice.localName() == accessory->getLocalName();
							bool isUuid = bleDevice.advertisedServiceUuid() == accessory->getServiceUuid();
							if(isName && isUuid){
								BLE.stopScan();
								accessory->setBleDevice(bleDevice);
								accessory->connect();
								break;
							}
						}
					}

					if(allAccessoriesConnected()){
						state = State::AllConnected;
					}
					else{
						state = State::StartScanning;
					}
				}
				break;
			}
			case State::AllConnected:
				if(!allAccessoriesConnected()){
					state = State::StartScanning;
				}
				break;
			default:
				state = State::StartScanning;
				break;
		}
	}
	void (*tick)() = tick_Implementation;

	static bool allAccessoriesConnected(){
		for(auto accessory : accessories){
			if(!accessory->isConnected()){
				return false;
			}
		}
		return true;
	}

	void disconnectAllAccessories_Implementation(){
		for(auto accessory : accessories){
			accessory->disconnect();
		}
	}
	void (*disconnectAllAccessories)() = disconnectAllAccessories_Implementation;

	static void send_Implementation(Command command){
		switch(command){
			case Command::TurnTable_TurnClockwise:
				turnTable.send(TurnTable::Command::TurnClockwise);
				break;
			case Command::TurnTable_TurnCounterClockwise:
				turnTable.send(TurnTable::Command::TurnCounterClockwise);
				break;
			case Command::Kodi_Up:
				kodiRemote.send(KodiRemote::Command::Up);
			default:
				break;
		}
	}
	void (*send)(Command command) = send_Implementation;
}
