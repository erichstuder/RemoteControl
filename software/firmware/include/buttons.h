#pragma once

#ifdef __cplusplus 
extern "C" {
#endif

typedef enum {
	ButtonId_SW1, ButtonId_SW2, ButtonId_SW3, ButtonId_SW4, ButtonId_SW5,
	ButtonId_SW6, ButtonId_SW7, ButtonId_SW8, ButtonId_SW9, ButtonId_SW10,
	ButtonId_SW11, ButtonId_SW12, ButtonId_SW13, ButtonId_SW14, ButtonId_SW15, ButtonId_None
} ButtonId;

extern void (*buttons_init)();
extern void (*buttons_tick)();
extern ButtonId (*buttons_getPressedEvent)();
extern void (*buttons_clearPressedEvent)();
bool buttons_isPressed(ButtonId id);
extern unsigned long (*buttons_getLastPressedMillis)();


/*namespace buttons{
	enum class Buttons{
		SW1,
		SW3,
		SW4,
		SW6,
		SW15,
		None,
	};

	extern void (*init)();
	extern void (*tick)();
	
	
	extern bool (*sw4_pressed)();
	extern bool (*sw6_pressed)();
}*/

#ifdef __cplusplus 
}
#endif
