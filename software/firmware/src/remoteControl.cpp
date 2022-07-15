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
		buttons_init();
	}

	void tick(void){
		bleRemoteHandler::tick();
		buttons_tick();
		handleButtons();

		if((millis()-buttons_getLastPressedMillis()) > 10000){
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
		switch(buttons_getPressedEvent()){
			case(ButtonId_SW1):
				buttons_clearPressedEvent();
				irRemoteHandler::send(irRemoteHandler::Command::HiFi_ToggleStandby);
				delay(100);
				break;
			case(ButtonId_SW3):
				buttons_clearPressedEvent();
				irRemoteHandler::send(irRemoteHandler::Command::TV_ToggleStandby);
				delay(100);
				break;
			case(ButtonId_SW4):
				while(buttons_isPressed(ButtonId_SW4)){
					buttons_clearPressedEvent();
					bleRemoteHandler::send(bleRemoteHandler::Command::TurnTable_TurnCounterClockwise);
				}
				break;
			case(ButtonId_SW6):
				while(buttons_isPressed(ButtonId_SW6)){
					buttons_clearPressedEvent();
					bleRemoteHandler::send(bleRemoteHandler::Command::TurnTable_TurnClockwise);
				}
				break;
			case(ButtonId_SW15):
				buttons_clearPressedEvent();
				bleRemoteHandler::send(bleRemoteHandler::Command::Kodi_Up);
				delay(100);
				break;
			default:
				//do nothing
				break;
		}
	}
}
