#include "remoteControl.h"
#include "buttons.h"
#include "irRemoteHandler.h"
#include "bleRemoteHandler.h"
#include "lowPower.h"
#include <Arduino.h>
#include <ArduinoBLE.h>

namespace remoteControl{
	enum class Command{
		None = 0x00,
		Clockwise = 0x01,
		CounterClockwise = 0x02,
		Up = 0x03,
		Down = 0x04,
		/*VolumeUp,
		VolumeDown,*/
	};


	// UUID considerations:
	// Avoid Base UUID.
	// Base UUID: 00000000-0000-1000-8000-00805F9B34FB
	// Don't use: XXXXXXXX-0000-1000-8000-00805F9B34FB
	// The UUIDs are left as is given by the code examples for the moment.
	static BLEService remoteControlService("19B10000-E8F2-537E-4F6C-D104768A1214");
	static BLEByteCharacteristic remoteControlCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLENotify);
	static BLEDevice remoteControl;

	static void handleButtons();

	void setup(void){
		lowPower::disableSensors();
		irRemoteHandler::init();
		//bleRemoteHandler::init();
		buttons::init();


		while(!BLE.begin());
		BLE.setLocalName("RemoteControl");
		BLE.setAdvertisedService(remoteControlService);
		remoteControlService.addCharacteristic(remoteControlCharacteristic);
		BLE.addService(remoteControlService);
		//remoteControlCharacteristic.writeValue((byte)Command::None); //set initial value. Vermutlich nutzlos, da ja notified wird.
		//remoteControl = BLE.central();
		BLE.advertise();
	}

	void tick(void){
		//bleRemoteHandler::tick();
		buttons::tick();
		handleButtons();

		/*if((millis()-buttons::getLastPressedMillis()) > 10000){
			//Disconnecting BLE devices before sleep will result in a faster reconnect after wake-up.
			//After a disconnect there is a delay necessary. Probably to let other tasks run and shutdown things correctly.
			bleRemoteHandler::disconnectAllAccessories();
			delay(1);

			digitalWrite(LED_PWR, LOW);
			lowPower::sleep();
			digitalWrite(LED_PWR, HIGH);
		}*/
	}

	static void handleButtons(){
		buttons::ButtonId button = buttons::getPressedEvent();
		switch(button){
			case(buttons::ButtonId::SW1):
				buttons::clearPressedEvent();
				irRemoteHandler::send(irRemoteHandler::Command::HiFi_ToggleStandby);
				delay(100);
				break;
			case(buttons::ButtonId::SW3):
				buttons::clearPressedEvent();
				irRemoteHandler::send(irRemoteHandler::Command::TV_ToggleStandby);
				delay(100);
				break;
			/*case(buttons::ButtonId::SW4):
				while(buttons::isPressed(button)){
					digitalWrite(LED_BUILTIN, HIGH);
					buttons::clearPressedEvent();
					bleRemoteHandler::send(bleRemoteHandler::Command::TurnTable_TurnCounterClockwise);
				}
				digitalWrite(LED_BUILTIN, LOW);
				break;
			case(buttons::ButtonId::SW6):
				while(buttons::isPressed(button)){
					digitalWrite(LED_BUILTIN, HIGH);
					buttons::clearPressedEvent();
					bleRemoteHandler::send(bleRemoteHandler::Command::TurnTable_TurnClockwise);
				}
				digitalWrite(LED_BUILTIN, LOW);
				break;*/
			case(buttons::ButtonId::SW8):
				remoteControlCharacteristic.writeValue((byte)Command::Up);
				/*buttons::clearPressedEvent();
				bleRemoteHandler::send(bleRemoteHandler::Command::Kodi_Up);*/
				delay(100);
				break;
			default:
				//do nothing
				break;
		}
	}
}
