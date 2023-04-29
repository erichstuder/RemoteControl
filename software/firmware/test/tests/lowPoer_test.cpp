#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "lowPower.h"
#include <Arduino.h>

TEST_GROUP(lowPower_test){
	void setup(void){}

	void teardown(void){
    	mock().checkExpectations();
		mock().clear();
	}
};

TEST(lowPower_test, disableSensors){
	mock().strictOrder();
	
	mock().expectOneCall("pinMode")
		.withParameter("pin", 33)
		.withParameter("mode", 1);
	mock().expectOneCall("digitalWrite")
		.withParameter("pin", 33)
		.withParameter("val", 0);
	
	mock().expectOneCall("pinMode")
		.withParameter("pin", 32)
		.withParameter("mode", 1);
	mock().expectOneCall("digitalWrite")
		.withParameter("pin", 32)
		.withParameter("val", 0);

	lowPower::disableSensors();
}

TEST(lowPower_test, sleep){
	NRF_TIMER2->TASKS_SHUTDOWN = 0;
	NRF_TIMER1->TASKS_SHUTDOWN = 0;
	NRF_TIMER0->TASKS_SHUTDOWN = 0;
	NRF_RTC1->TASKS_STOP = 0;
	NRF_CLOCK->TASKS_LFCLKSTOP = 0;
	NRF_CLOCK->TASKS_HFCLKSTOP = 0;
	NRF_RADIO->TASKS_DISABLE = 0;
	mock().expectOneCall("__WFI");
	NRF_RADIO->TASKS_START = 0;
	NRF_CLOCK->TASKS_HFCLKSTART = 0;
	NRF_CLOCK->TASKS_LFCLKSTART = 0;
	NRF_RTC1->TASKS_START = 0;
	NRF_TIMER0->TASKS_START = 0;
	NRF_TIMER1->TASKS_START = 0;
	NRF_TIMER2->TASKS_START = 0;

	lowPower::sleep();

	CHECK_EQUAL(1, NRF_TIMER2->TASKS_SHUTDOWN);
	CHECK_EQUAL(1, NRF_TIMER1->TASKS_SHUTDOWN);
	CHECK_EQUAL(1, NRF_TIMER0->TASKS_SHUTDOWN);
	CHECK_EQUAL(1, NRF_RTC1->TASKS_STOP);
	CHECK_EQUAL(1, NRF_CLOCK->TASKS_LFCLKSTOP);
	CHECK_EQUAL(1, NRF_CLOCK->TASKS_HFCLKSTOP);
	CHECK_EQUAL(1, NRF_RADIO->TASKS_DISABLE);
	//__WFI
	CHECK_EQUAL(1, NRF_RADIO->TASKS_START);
	CHECK_EQUAL(1, NRF_CLOCK->TASKS_HFCLKSTART);
	CHECK_EQUAL(1, NRF_CLOCK->TASKS_LFCLKSTART);
	CHECK_EQUAL(1, NRF_RTC1->TASKS_START);
	CHECK_EQUAL(1, NRF_TIMER0->TASKS_START);
	CHECK_EQUAL(1, NRF_TIMER1->TASKS_START);
	CHECK_EQUAL(1, NRF_TIMER2->TASKS_START);
}
