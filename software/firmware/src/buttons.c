#include <Arduino.h>
#include "buttons.h"

typedef struct{
	ButtonId id;
	pin_size_t pin;
	bool hadPressedInterrupt;
	void (*pressedInterrupt)();
}Button;

static Button* getButtonById(ButtonId id);

static void sw1_pressedInterrupt() { getButtonById(ButtonId_SW1)->hadPressedInterrupt  = true; }
static void sw2_pressedInterrupt() { getButtonById(ButtonId_SW2)->hadPressedInterrupt  = true; }
static void sw3_pressedInterrupt() { getButtonById(ButtonId_SW3)->hadPressedInterrupt  = true; }
static void sw4_pressedInterrupt() { getButtonById(ButtonId_SW4)->hadPressedInterrupt  = true; }
static void sw5_pressedInterrupt() { getButtonById(ButtonId_SW5)->hadPressedInterrupt  = true; }
static void sw6_pressedInterrupt() { getButtonById(ButtonId_SW6)->hadPressedInterrupt  = true; }
static void sw7_pressedInterrupt() { getButtonById(ButtonId_SW7)->hadPressedInterrupt  = true; }
static void sw8_pressedInterrupt() { getButtonById(ButtonId_SW8)->hadPressedInterrupt  = true; }
static void sw9_pressedInterrupt() { getButtonById(ButtonId_SW9)->hadPressedInterrupt  = true; }
static void sw10_pressedInterrupt(){ getButtonById(ButtonId_SW10)->hadPressedInterrupt = true; }
static void sw11_pressedInterrupt(){ getButtonById(ButtonId_SW11)->hadPressedInterrupt = true; }
static void sw12_pressedInterrupt(){ getButtonById(ButtonId_SW12)->hadPressedInterrupt = true; }
static void sw13_pressedInterrupt(){ getButtonById(ButtonId_SW13)->hadPressedInterrupt = true; }
static void sw14_pressedInterrupt(){ getButtonById(ButtonId_SW14)->hadPressedInterrupt = true; }
static void sw15_pressedInterrupt(){ getButtonById(ButtonId_SW15)->hadPressedInterrupt = true; }

static Button buttons[] = {
	{.id=ButtonId_SW1,  .pin=D4,  .hadPressedInterrupt=false, .pressedInterrupt = sw1_pressedInterrupt },
	{.id=ButtonId_SW2,  .pin=D9,  .hadPressedInterrupt=false, .pressedInterrupt = sw2_pressedInterrupt },
	{.id=ButtonId_SW3,  .pin=A6,  .hadPressedInterrupt=false, .pressedInterrupt = sw3_pressedInterrupt },
	{.id=ButtonId_SW4,  .pin=D3,  .hadPressedInterrupt=false, .pressedInterrupt = sw4_pressedInterrupt },
	{.id=ButtonId_SW5,  .pin=D8,  .hadPressedInterrupt=false, .pressedInterrupt = sw5_pressedInterrupt },
	{.id=ButtonId_SW6,  .pin=A5,  .hadPressedInterrupt=false, .pressedInterrupt = sw6_pressedInterrupt },
	{.id=ButtonId_SW7,  .pin=D2,  .hadPressedInterrupt=false, .pressedInterrupt = sw7_pressedInterrupt },
	{.id=ButtonId_SW8,  .pin=D7,  .hadPressedInterrupt=false, .pressedInterrupt = sw8_pressedInterrupt },
	{.id=ButtonId_SW9,  .pin=D12, .hadPressedInterrupt=false, .pressedInterrupt = sw9_pressedInterrupt },
	{.id=ButtonId_SW10, .pin=D0,  .hadPressedInterrupt=false, .pressedInterrupt = sw10_pressedInterrupt},
	{.id=ButtonId_SW11, .pin=D6,  .hadPressedInterrupt=false, .pressedInterrupt = sw11_pressedInterrupt},
	{.id=ButtonId_SW12, .pin=D11, .hadPressedInterrupt=false, .pressedInterrupt = sw12_pressedInterrupt},
	{.id=ButtonId_SW13, .pin=D1,  .hadPressedInterrupt=false, .pressedInterrupt = sw13_pressedInterrupt},
	{.id=ButtonId_SW14, .pin=D5,  .hadPressedInterrupt=false, .pressedInterrupt = sw14_pressedInterrupt},
	{.id=ButtonId_SW15, .pin=D10, .hadPressedInterrupt=false, .pressedInterrupt = sw15_pressedInterrupt}
};


static void updateLastPressedTime();
static ButtonId pressedButton = ButtonId_None;
static unsigned long lastPressedMillis = 0;


static void init_Implementation(){
	for(unsigned int n=0; n<sizeof(buttons)/sizeof(buttons[0]) ; n++){
		Button button = buttons[n];
		attachInterrupt(button.pin, button.pressedInterrupt, FALLING);
	}
}
void (*buttons_init)() = init_Implementation;


static void tick_Implementation(){
	for(unsigned int n=0; n<sizeof(buttons)/sizeof(buttons[0]) ; n++){
		Button button = buttons[n];
		if(button.hadPressedInterrupt){
			button.hadPressedInterrupt = false;
			pressedButton = button.id;
			updateLastPressedTime();
		}
	}
}
void (*buttons_tick)() = tick_Implementation;


static ButtonId getPressedEvent_Implementation(){
	return pressedButton;
}
ButtonId (*buttons_getPressedEvent)() = getPressedEvent_Implementation;


static void clearPressedEvent_Implementation(){
	pressedButton = ButtonId_None;
}
void (*buttons_clearPressedEvent)() = clearPressedEvent_Implementation;


static Button* getButtonById(ButtonId id){
	for(unsigned int n=0; n<sizeof(buttons)/sizeof(buttons[0]) ; n++){
		Button* button = &(buttons[n]);
		if(button->id == id){
			return button;
		}
	}
	return NULL;
}


bool buttons_isPressed(ButtonId id){
	Button* button = getButtonById(id);
	if(button == NULL){
		return false;
	}
	if(digitalRead(button->pin) == LOW){
		updateLastPressedTime();
		return true;
	}
	return false;
}


static void updateLastPressedTime(){
	lastPressedMillis = millis();
}


static unsigned long getLastPressedMillis_Implementation(){
	return lastPressedMillis;
}
unsigned long (*buttons_getLastPressedMillis)() = getLastPressedMillis_Implementation;
