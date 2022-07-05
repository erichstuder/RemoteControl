#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "irRemoteHandler.h"
#include "MELECTRONIC__MC_MI_1212__IrRemote.h"
#include "FINLUX__32FLE845_Eco__IrRemote.h"

static const uint8_t IrPin = 21;

static void melectronic_init_mock(	MELECTRONIC__MC_MI_1212::InfraRed_on infraRed_on,
									MELECTRONIC__MC_MI_1212::InfraRed_off infraRed_off, 
									MELECTRONIC__MC_MI_1212::WaitMicroseconds waitUs){
	mock().actualCall("melectronic_init_mock");
	mock().setData("melectronic_infraRed_on", infraRed_on);
	mock().setData("melectronic_infraRed_off", infraRed_off);
	mock().setData("melectronic_waitUs", (void(*)())waitUs);
}

static void finlux_init_mock(	FINLUX__32FLE845_Eco::InfraRed_on infraRed_on,
								FINLUX__32FLE845_Eco::InfraRed_off infraRed_off,
								FINLUX__32FLE845_Eco::WaitMicroseconds waitUs){
	mock().actualCall("finlux_init_mock");
	mock().setData("finlux_infraRed_on", infraRed_on);
	mock().setData("finlux_infraRed_off", infraRed_off);
	mock().setData("finlux_waitUs", (void(*)())waitUs);
}

static void melectronic_send_mock(MELECTRONIC__MC_MI_1212::Command command){
	mock().actualCall("melectronic_send_mock")
		.withIntParameter("command", (int)command);
}

static void finlux_send_mock(FINLUX__32FLE845_Eco::Command command){
	mock().actualCall("finlux_send_mock")
		.withIntParameter("command", (int)command);
}

static void expect_irPinWrite(uint8_t val){
	mock().expectOneCall("digitalWrite")
		.withParameter("pin", IrPin)
		.withParameter("val", val);
}


TEST_GROUP(IrRemoteHandler_test){
	void setup(void){
		UT_PTR_SET(MELECTRONIC__MC_MI_1212::init, melectronic_init_mock);
		UT_PTR_SET(FINLUX__32FLE845_Eco::init, finlux_init_mock);
		UT_PTR_SET(MELECTRONIC__MC_MI_1212::send, melectronic_send_mock);
		UT_PTR_SET(FINLUX__32FLE845_Eco::send, finlux_send_mock);
	}
	void teardown(void){
    	mock().checkExpectations();
		mock().clear();
	}
};

TEST(IrRemoteHandler_test, init){
	mock().strictOrder();
	mock().expectOneCall("melectronic_init_mock");
	mock().expectOneCall("finlux_init_mock");
	mock().expectOneCall("pinMode")
		.withParameter("pin", IrPin)
		.withParameter("mode", 1);

	irRemoteHandler::init();
}

static void init_noMock(){
	mock().disable();
	irRemoteHandler::init();
	mock().enable();
}

TEST(IrRemoteHandler_test, melectronic_infraRed_on){
	init_noMock();

	using namespace MELECTRONIC__MC_MI_1212;
	InfraRed_on infraRed_on = (InfraRed_on)mock().getData("melectronic_infraRed_on").getFunctionPointerValue();

	expect_irPinWrite(1);

	infraRed_on();
}

TEST(IrRemoteHandler_test, finlux_infraRed_on){
	init_noMock();

	using namespace FINLUX__32FLE845_Eco;
	InfraRed_on infraRed_on = (InfraRed_on)mock().getData("finlux_infraRed_on").getFunctionPointerValue();

	expect_irPinWrite(1);

	infraRed_on();
}

TEST(IrRemoteHandler_test, melectronic_infraRed_off){
	init_noMock();

	using namespace MELECTRONIC__MC_MI_1212;
	InfraRed_off infraRed_off = (InfraRed_off)mock().getData("melectronic_infraRed_off").getFunctionPointerValue();

	expect_irPinWrite(0);

	infraRed_off();
}

TEST(IrRemoteHandler_test, finlux_infraRed_off){
	init_noMock();

	using namespace FINLUX__32FLE845_Eco;
	InfraRed_off infraRed_off = (InfraRed_off)mock().getData("finlux_infraRed_off").getFunctionPointerValue();

	expect_irPinWrite(0);

	infraRed_off();
}

TEST(IrRemoteHandler_test, melectronic_waitUs){
	init_noMock();

	using namespace MELECTRONIC__MC_MI_1212;
	WaitMicroseconds waitUs = (WaitMicroseconds)mock().getData("melectronic_waitUs").getFunctionPointerValue();

	//expect no calls
	waitUs(42);
}

TEST(IrRemoteHandler_test, finlux_waitUs){
	init_noMock();

	using namespace FINLUX__32FLE845_Eco;
	WaitMicroseconds waitUs = (WaitMicroseconds)mock().getData("finlux_waitUs").getFunctionPointerValue();

	//expect no calls
	waitUs(123);
}

TEST(IrRemoteHandler_test, HiFi_ToggleStandby){
	mock().expectOneCall("melectronic_send_mock")
		.withIntParameter("command", (int)MELECTRONIC__MC_MI_1212::Command::ToggleStandby);

	irRemoteHandler::send(irRemoteHandler::Command::HiFi_ToggleStandby);
}

TEST(IrRemoteHandler_test, TV_ToggleStandby){
	mock().expectOneCall("finlux_send_mock")
		.withIntParameter("command", (int)FINLUX__32FLE845_Eco::Command::ToggleStandby);

	irRemoteHandler::send(irRemoteHandler::Command::TV_ToggleStandby);
}

TEST(IrRemoteHandler_test, send_invalidCommand){
	//expect no call
	irRemoteHandler::send((irRemoteHandler::Command)42);
}
