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
		switch(buttons::getPressedEvent()){
			case(buttons::ButtonId::SW11):
				buttons::clearPressedEvent();
				irRemoteHandler::send(irRemoteHandler::Command::HiFi_ToggleStandby);
				delay(100);
				break;
			case(buttons::ButtonId::SW12):
				buttons::clearPressedEvent();
				irRemoteHandler::send(irRemoteHandler::Command::TV_ToggleStandby);
				delay(100);
				break;
			case(buttons::ButtonId::SW14):
				while(buttons::isPressed(buttons::ButtonId::SW14)){
					buttons::clearPressedEvent();
					bleRemoteHandler::send(bleRemoteHandler::Command::TurnTable_TurnCounterClockwise);
				}
				break;
			case(buttons::ButtonId::SW15):
				while(buttons::isPressed(buttons::ButtonId::SW15)){
					buttons::clearPressedEvent();
					bleRemoteHandler::send(bleRemoteHandler::Command::TurnTable_TurnClockwise);
				}
				break;
			case(buttons::ButtonId::SW13):
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
