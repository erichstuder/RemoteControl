#include "remoteControl.h"
#include "buttons.h"
#include "irRemoteHandler.h"
#include "turnTable.h"
#include "lowPower.h"
#include <Arduino.h>

namespace remoteControl{
	static void handleButtons();

	void setup(void){
		lowPower::disableSensors();

		irRemoteHandler::init();
		turnTable::init();
		buttons::init();
	}

	void tick(void){
		turnTable::tick();
		buttons::tick();
		handleButtons();

		if((millis()-buttons::getLastPressedMillis()) > 10000){
			//Disconnecting turnTable before sleep will result in a faster reconnect after wake-up.
			//After a disconnect there is a delay necessary. Probably to let other tasks run and shutdown things correctly.
			turnTable::disconnect();
			delay(1);

			digitalWrite(LED_PWR, LOW);
			lowPower::sleep();
			digitalWrite(LED_PWR, HIGH);
		}
	}

	static void handleButtons(){
		switch(buttons::getPressedEvent()){
			case(buttons::Buttons::SW4):
				while(buttons::sw4_pressed() && turnTable::isConnected()){
					buttons::clearPressedEvent();
					turnTable::sendCommand(turnTable::Command::TurnCounterClockWise);
				}
				break;
			case(buttons::Buttons::SW6):
				while(buttons::sw6_pressed() && turnTable::isConnected()){
					buttons::clearPressedEvent();
					turnTable::sendCommand(turnTable::Command::TurnClockWise);
				}
				break;
			case(buttons::Buttons::SW1):
				buttons::clearPressedEvent();
				irRemoteHandler::send(irRemoteHandler::Command::HiFi_ToggleStandby);
				delay(100);
				break;
			case(buttons::Buttons::SW3):
				buttons::clearPressedEvent();
				irRemoteHandler::send(irRemoteHandler::Command::TV_ToggleStandby);
				delay(100);
				break;
			default:
				//do nothing
				break;
		}
	}
}
