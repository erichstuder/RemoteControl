#include <Arduino.h>
#include "buttons.h"

namespace buttons{
	static const pin_size_t SW1_pin = D4;
	static const pin_size_t SW3_pin = A6;
	static const pin_size_t SW4_pin = D3;
	static const pin_size_t SW6_pin = A5;
	static bool sw1_isPressedInterrupt = false;
	static bool sw3_isPressedInterrupt = false;
	static bool sw4_isPressedInterrupt = false;
	static bool sw6_isPressedInterrupt = false;
	static Buttons pressedButton = Buttons::None;
	static unsigned long lastPressedMillis = 0;

	static void sw1_pressedInterrupt();
	static void sw3_pressedInterrupt();
	static void sw4_pressedInterrupt();
	static void sw6_pressedInterrupt();
	static void updateLastPressedTime();

	static void init_Implementation(){
		pinMode(SW1_pin, INPUT_PULLUP);
		pinMode(SW3_pin, INPUT_PULLUP);
		pinMode(SW4_pin, INPUT_PULLUP);
		pinMode(SW6_pin, INPUT_PULLUP);

		attachInterrupt(digitalPinToInterrupt(SW1_pin), sw1_pressedInterrupt, FALLING);
		attachInterrupt(digitalPinToInterrupt(SW3_pin), sw3_pressedInterrupt, FALLING);
		attachInterrupt(digitalPinToInterrupt(SW4_pin), sw4_pressedInterrupt, FALLING);
		attachInterrupt(digitalPinToInterrupt(SW6_pin), sw6_pressedInterrupt, FALLING);
	}
	void (*init)() = init_Implementation;

	static void tick_Implementation(){
		if(sw1_isPressedInterrupt){
			sw1_isPressedInterrupt = false;
			pressedButton = Buttons::SW1;
			updateLastPressedTime();
		}
		else if(sw3_isPressedInterrupt){
			sw3_isPressedInterrupt = false;
			pressedButton = Buttons::SW3;
			updateLastPressedTime();
		}
		else if(sw4_isPressedInterrupt){
			sw4_isPressedInterrupt = false;
			pressedButton = Buttons::SW4;
			updateLastPressedTime();
		}
		else if(sw6_isPressedInterrupt){
			sw6_isPressedInterrupt = false;
			pressedButton = Buttons::SW6;
			updateLastPressedTime();
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

	static void updateLastPressedTime(){
		lastPressedMillis = millis();
	}

	static unsigned long getLastPressedMillis_Implementation(){
		return lastPressedMillis;
	}
	unsigned long (*getLastPressedMillis)() = getLastPressedMillis_Implementation;
}
