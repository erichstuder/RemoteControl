#include "remoteControl.h"
#include "buttons.h"
#include "irRemoteHandler.h"
#include "bleRemoteHandler.h"
#include "lowPower.h"
#include <Arduino.h>

namespace remoteControl{
	static void handleButtons();

	void setup(void){
		lowPower::disableSensors();
		irRemoteHandler::init();
		bleRemoteHandler::init();
		buttons::init();
	}

	void tick(void){
		bleRemoteHandler::tick();
		buttons::tick();
		handleButtons();

		if((millis()-buttons::getLastPressedMillis()) > 10000){
			//Disconnecting BLE devices before sleep will result in a faster reconnect after wake-up.
			//After a disconnect there is a delay necessary. Probably to let other tasks run and shutdown things correctly.
			bleRemoteHandler::disconnectAllAccessories();
			delay(1);

			digitalWrite(LED_PWR, LOW);
			lowPower::sleep();
			digitalWrite(LED_PWR, HIGH);
		}
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
			case(buttons::ButtonId::SW4):
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
				break;
			case(buttons::ButtonId::SW8):
				buttons::clearPressedEvent();
				bleRemoteHandler::send(bleRemoteHandler::Command::Kodi_Up);
				delay(100);
				break;
			default:
				//do nothing
				break;
		}
	}
}
