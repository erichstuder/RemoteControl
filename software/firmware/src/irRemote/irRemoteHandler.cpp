#include "irRemoteHandler.h"
#include "MELECTRONIC__MC_MI_1212__IrRemote.h"
#include "FINLUX__32FLE845_Eco__IrRemote.h"

#include <Arduino.h>

namespace irRemoteHandler{
	static const pin_size_t IrPin = A7;

	static void infraRed_on();
	static void infraRed_off();
	static void waitUs(unsigned int microSeconds);

	static void init_Implementation(){
		MELECTRONIC__MC_MI_1212::init(infraRed_on, infraRed_off, waitUs);
		FINLUX__32FLE845_Eco::init(infraRed_on, infraRed_off, waitUs);
		pinMode(IrPin, OUTPUT);
	}
	void (*init)() = init_Implementation;
	
	static void send_Implementation(Command command){
		switch(command){
			case(Command::HiFi_ToggleStandby):
				MELECTRONIC__MC_MI_1212::send(MELECTRONIC__MC_MI_1212::Command::ToggleStandby);
				break;
			case(Command::TV_ToggleStandby):
				FINLUX__32FLE845_Eco::send(FINLUX__32FLE845_Eco::Command::ToggleStandby);
				break;
			default:
				//do nothing
				break;
		}
	}
	void (*send)(Command command) = send_Implementation;

	static void infraRed_on(){
		digitalWrite(IrPin, HIGH);
	}

	static void infraRed_off(){
		digitalWrite(IrPin, LOW);
	}

	static void waitUs(unsigned int microSeconds){
		//The microsecond must be more accurate than delayMicroseconds is.
		//This is a bit hacky but more accurate.
		for(uint16_t n = 0; n < microSeconds; n++){
			volatile uint16_t i;
			for(i=0; i<4; i++){
			}
		}
	}
}
