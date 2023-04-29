#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "buttons.h"

typedef void(*InterruptFunction)();

static const uint8_t SW1_pin = 4;
static const uint8_t SW3_pin = 20;
static const uint8_t SW4_pin = 3;
static const uint8_t SW6_pin = 19;
static const uint8_t Pins[] = {SW1_pin, SW3_pin, SW4_pin, SW6_pin};

TEST_GROUP(buttons_test){
	void setup(void){}
	void teardown(void){
    	mock().checkExpectations();
		mock().clear();
	}
};


/*static void expect_pinMode(uint8_t pin){
	mock().expectOneCall("pinMode")
		.withParameter("pin", pin)
		.withParameter("mode", 2);
}

static void expect_attachInterrupt(uint8_t pin){
	mock().expectOneCall("digitalPinToInterrupt")
		.withParameter("pin", pin);
	mock().expectOneCall("attachInterrupt")
		.withParameter("interruptNum", pin)
		.withParameter("mode", 3);
}*/

static InterruptFunction getPinInterruptFunction(uint8_t pinNumber){
	const char* DataName = ("func"+std::to_string(pinNumber)).c_str();
	return mock().getData(DataName).getFunctionPointerValue();
}


/*TEST(buttons_test, init){
	mock().strictOrder();
	for(uint8_t n=0; n<sizeof(Pins)/sizeof(Pins[0]); n++){
		expect_pinMode(Pins[n]);
	}
	mock().strictOrder();
	for(uint8_t n=0; n<sizeof(Pins)/sizeof(Pins[0]); n++){
		expect_attachInterrupt(Pins[n]);
	}

	buttons::init();
}*/

TEST(buttons_test, tick_noEvent){
	buttons::tick();
	mock().expectNoCall("millis");
}

TEST(buttons_test, lastPressedTime){
	mock().ignoreOtherCalls();
	buttons::init();

	for(uint8_t n=0; n<sizeof(Pins)/sizeof(Pins[0]); n++){
		InterruptFunction func = getPinInterruptFunction(Pins[n]);

		func();
		mock().expectOneCall("millis")
			.andReturnValue(n);
		buttons::tick();
		CHECK_EQUAL(n, buttons::getLastPressedMillis());
	}
}

TEST(buttons_test, event){
	static const buttons::ButtonId Buttons[] = {	buttons::ButtonId::SW1,
													buttons::ButtonId::SW3,
													buttons::ButtonId::SW4,
													buttons::ButtonId::SW6,};

	mock().ignoreOtherCalls();
	buttons::init();

	for(uint8_t n=0; n<sizeof(Pins)/sizeof(Pins[0]); n++){
		InterruptFunction func = getPinInterruptFunction(Pins[n]);

		CHECK(buttons::ButtonId::None == buttons::getPressedEvent());
		func();
		CHECK(buttons::ButtonId::None == buttons::getPressedEvent());
		buttons::tick();
		CHECK(Buttons[n] == buttons::getPressedEvent());
		buttons::clearPressedEvent();
		CHECK(buttons::ButtonId::None == buttons::getPressedEvent());
	}
}

TEST(buttons_test, sw4_pressed_false){
	mock().expectOneCall("digitalRead")
		.withParameter("pin", SW4_pin)
		.andReturnValue(1);
	CHECK_FALSE(buttons::isPressed(buttons::ButtonId::SW4));
}

TEST(buttons_test, sw4_pressed_true){
	const unsigned long Time = 253;
	mock().expectOneCall("digitalRead")
		.withParameter("pin", SW4_pin)
		.andReturnValue(0);
	mock().expectOneCall("millis")
		.andReturnValue(Time);

	CHECK(buttons::isPressed(buttons::ButtonId::SW4));

	CHECK_EQUAL(Time, buttons::getLastPressedMillis());
}

TEST(buttons_test, sw6_pressed_false){
	mock().expectOneCall("digitalRead")
		.withParameter("pin", SW6_pin)
		.andReturnValue(1);
	CHECK_FALSE(buttons::isPressed(buttons::ButtonId::SW6));
}

TEST(buttons_test, sw6_pressed_true){
	const unsigned long Time = 25;
	mock().expectOneCall("digitalRead")
		.withParameter("pin", SW6_pin)
		.andReturnValue(0);
	mock().expectOneCall("millis")
		.andReturnValue(Time);

	CHECK(buttons::isPressed(buttons::ButtonId::SW6));

	CHECK_EQUAL(Time, buttons::getLastPressedMillis());
}
