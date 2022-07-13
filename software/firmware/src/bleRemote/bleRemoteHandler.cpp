#include <Arduino.h>
#include <ArduinoBLE.h>
//#include "bleRemoteHandler.h"
#include "turnTable.h"
#include "BleUsbKeyboard.h"
#include "BleRemoteDevice_interface.h"

//TODO: evtl. testen, ob man nach mehreren Service UUIDs gleichzeitig scannen kann.
//TODO: für die einzelnen Geräte ein Interface erstellen z.B. bleRemoteDevice_interface.cpp

namespace bleRemoteHandler{
	static BleUsbKeyboard usbKeyboard;


	enum class State{
		StartScanning,
		Scanning,
		FullyConnected,
	};

	static State state;

	static bool connectToPeripheral(BleRemoteDevice_interface* device);

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

				//TODO: use polymorphism
				if(bleDevice){
					bool isName = bleDevice.localName() == usbKeyboard.getLocalName();
					bool isUuid = bleDevice.advertisedServiceUuid() == usbKeyboard.getServiceUuid();
					if(isName && isUuid){
						usbKeyboard.bleDevice = bleDevice;
						BLE.stopScan();
						connectToPeripheral(&usbKeyboard);
						break;
					}

					/*bool isName = bleDevice.localName() == turnTable::getLocalName();
					bool isUuid = bleDevice.advertisedServiceUuid() == turnTable::getServiceUuid();
					if(isName && isUuid){
						turnTable::bleDevice = bleDevice;
						BLE.stopScan();
						connectToPeripheral(turnTable::bleDevice);
						break;
					}
					
					isName = devibleDevicece.localName() == bleUsbKeyboard::getLocalName();
					isUuid = bleDevice.advertisedServiceUuid() == bleUsbKeyboard::getServiceUuid();
					if(isName && isUuid){
						bleUsbKeyboard::bleDevice = bleDevice;
						BLE.stopScan();
						connectToPeripheral(bleUsbKeyboard::bleDevice);
						break;
					}*/
				}
				


				/*if(device && device.localName() == "TurnTable"){
					BLE.stopScan();
					if(connectToPeripheral()){
						state = State::FullyConnected;
					}
					else{
						state = State::StartScanning;
					}
				}*/
				break;}
			case State::FullyConnected:
				/*if(!device.connected()){
					state = State::StartScanning;
				}*/
				break;
			default:
				state = State::StartScanning;
				break;
		}
	}
	void (*tick)() = tick_Implementation;

	static bool connectToPeripheral(BleRemoteDevice_interface* device){
		if(!device->bleDevice.connect()){
			return false;
		}
		
		if(!device->bleDevice.discoverAttributes()){
			device->bleDevice.disconnect();
			return false;
		}

		//BLECharacteristic characteristic = device.bleDevice.characteristic("19b10001-e8f2-537e-4f6c-d104768a1214");
		BLECharacteristic characteristic = device->bleDevice.characteristic(device->getCharacteristicUuid().c_str());

		if(!characteristic){
			device->bleDevice.disconnect();
			return false;
		}

		if(!characteristic.canWrite()){
			device->bleDevice.disconnect();
			return false;
		}

		device->bleCharacteristic = characteristic;

		return true;
	}
}
