#include <Arduino.h>
#include "buttons.h"
#include <map>

namespace buttons{
	enum class SwitchId{
		SW1,
		SW2,
		SW3,
		SW4,
		SW5,
		SW6,
		SW7,
		SW8,
		SW9,
		SW10,
		SW11,
		SW12,
		SW13,
		SW14,
		SW15
	};

	struct Switch{
		const pin_size_t Pin;
		bool isPressedInterrupt = false;

		/*void pressedInterrupt(){
			isPressedInterrupt = true;
		}*/

		Switch(pin_size_t pin) : Pin(pin){
			pinMode(Pin, INPUT_PULLUP);
			attachInterrupt() digitalPinToInterrupt(Pin), [this](){isPressedInterrupt = true;}, FALLING);
		}

		void isPressed(){
			digitalRead(Pin);
		}
	};

	std::map<SwitchId, Switch> switches = {
		SwitchId::SW1,  Switch(D4),
		SwitchId::SW2,  Switch(D9),
		SwitchId::SW3,  Switch(A6),
		SwitchId::SW4,  Switch(D3),
		SwitchId::SW5,  Switch(D8),
		SwitchId::SW6,  Switch(A5),
		SwitchId::SW7,  Switch(D2),
		SwitchId::SW8,  Switch(D7),
		SwitchId::SW9,  Switch(D12),
		SwitchId::SW10, Switch(D0),
		SwitchId::SW11, Switch(D6),
		SwitchId::SW12, Switch(D11),
		SwitchId::SW13, Switch(D1),
		SwitchId::SW14, Switch(D5),
		SwitchId::SW15, Switch(D10)
	};

	/*static bool sw1_isPressedInterrupt = false;
	static bool sw2_isPressedInterrupt = false;
	static bool sw3_isPressedInterrupt = false;
	static bool sw4_isPressedInterrupt = false;
	static bool sw5_isPressedInterrupt = false;
	static bool sw6_isPressedInterrupt = false;
	static bool sw7_isPressedInterrupt = false;
	static bool sw8_isPressedInterrupt = false;
	static bool sw9_isPressedInterrupt = false;
	static bool sw10_isPressedInterrupt = false;
	static bool sw11_isPressedInterrupt = false;
	static bool sw12_isPressedInterrupt = false;
	static bool sw13_isPressedInterrupt = false;
	static bool sw14_isPressedInterrupt = false;
	static bool sw15_isPressedInterrupt = false;

	static void sw1_pressedInterrupt();
	static void sw2_pressedInterrupt();
	static void sw3_pressedInterrupt();
	static void sw4_pressedInterrupt();
	static void sw5_pressedInterrupt();
	static void sw6_pressedInterrupt();
	static void sw7_pressedInterrupt();
	static void sw8_pressedInterrupt();
	static void sw9_pressedInterrupt();
	static void sw10_pressedInterrupt();
	static void sw11_pressedInterrupt();
	static void sw12_pressedInterrupt();
	static void sw13_pressedInterrupt();
	static void sw14_pressedInterrupt();
	static void sw15_pressedInterrupt();*/

	static void updateLastPressedTime();
	static Buttons pressedButton = Buttons::None;
	static unsigned long lastPressedMillis = 0;
	

	static void init_Implementation(){}
	void (*init)() = init_Implementation;

	static void tick_Implementation(){
		for(auto sw : switches){
			if(sw.isPressedInterrupt){
				sw.isPressedInterrupt = false;
				pressedButton = sw.Id;
				updateLastPressedTime();
			}
		}
	}
	void (*tick)() = tick_Implementation;

	static Buttons getPressedEvent_Implementation(){
		return pressedButton;
	}
	Buttons (*getPressedEvent)() = getPressedEvent_Implementation;

	static void clearPressedEvent_Implementation(){
		pressedButton = Buttons::None;
	}
	void (*clearPressedEvent)() = clearPressedEvent_Implementation;


	bool isPressed(SwitchId id){
		if
	}

	static bool sw4_pressed_Implementation(){
		PinStatus pinStatus = digitalRead(SW4_pin);
		if(pinStatus == LOW){
			updateLastPressedTime();
			return true;
		}
		else{
			return false;
		}
	}
	bool (*sw4_pressed)() = sw4_pressed_Implementation;

	static bool sw6_pressed_Implementation(){
		PinStatus pinStatus = digitalRead(SW6_pin);
		if(pinStatus == LOW){
			updateLastPressedTime();
			return true;
		}
		else{
			return false;
		}
	}
	bool (*sw6_pressed)() = sw6_pressed_Implementation;

	static bool sw15_pressed_Implementation(){
		PinStatus pinStatus = digitalRead(SW15_pin);
		if(pinStatus == LOW){
			updateLastPressedTime();
			return true;
		}
		else{
			return false;
		}
	}
	bool (*sw15_pressed)() = sw15_pressed_Implementation;


	static void sw4_pressedInterrupt(){
		sw4_isPressedInterrupt = true;
	}

	static void sw6_pressedInterrupt(){
		sw6_isPressedInterrupt = true;
	}

	static void sw1_pressedInterrupt(){
		sw1_isPressedInterrupt = true;
	}

	static void sw3_pressedInterrupt(){
		sw3_isPressedInterrupt = true;
	}

	static void sw15_pressedInterrupt(){
		sw15_isPressedInterrupt = true;
	}

	static void updateLastPressedTime(){
		lastPressedMillis = millis();
	}

	static unsigned long getLastPressedMillis_Implementation(){
		return lastPressedMillis;
	}
	unsigned long (*getLastPressedMillis)() = getLastPressedMillis_Implementation;
}
