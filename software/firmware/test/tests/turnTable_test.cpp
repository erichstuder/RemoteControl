/*#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "turnTable.h"

static void expect_Scanning_until_available(){
	mock().expectOneCall("BLELocalDevice::available");
}

static void expect_Scanning_until_peripheral(){
	expect_Scanning_until_available();
	mock().expectOneCall("BLEDevice::operator bool")
		.andReturnValue(true);
}

static void expect_Scanning_until_stopScan(){
	expect_Scanning_until_peripheral();
	mock().expectOneCall("BLEDevice::localName")
		.andReturnValue("TurnTable");
	mock().expectOneCall("BLELocalDevice::stopScan");
}

static void expect_Scanning_until_connect(){
	expect_Scanning_until_stopScan();
	mock().expectOneCall("BLEDevice::connect")
		.andReturnValue(true);
}

static void expect_Scanning_until_discoverAttributes(){
	expect_Scanning_until_connect();
	mock().expectOneCall("BLEDevice::discoverAttributes")
		.andReturnValue(true);
	mock().expectOneCall("BLEDevice::characteristic")
		.withStringParameter("uuid", "19b10001-e8f2-537e-4f6c-d104768a1214");
}

static void expect_Scanning_until_turnCharacteristic(){
	expect_Scanning_until_discoverAttributes();
	mock().expectOneCall("BLECharacteristic::operator bool")
		.andReturnValue(true);
}

static void enterState_Scanning(){
	mock().disable();
	turnTable::tick();
	mock().enable();
}

static void enterState_Connected(){
	enterState_Scanning();

	mock().disable();
	turnTable::tick();
	mock().enable();
}


TEST_GROUP(turnTable_test){
	void setup(void){
		mock().disable();
		turnTable::init();
		mock().enable();
		mock().strictOrder();
	}
	void teardown(void){
    	mock().checkExpectations();
		mock().clear();
	}
};

TEST(turnTable_test, init){
	mock().expectOneCall("BLELocalDevice::begin");
	turnTable::init();
}

TEST(turnTable_test, tick_StartScanning){
	mock().expectOneCall("BLELocalDevice::scanForUuid")
		.withStringParameter("uuid", "19b10000-e8f2-537e-4f6c-d104768a1214")
		.withBoolParameter("withDuplicates", true);
	turnTable::tick();
}

TEST(turnTable_test, tick_Scanning_notFound){
	expect_Scanning_until_available();
	mock().expectOneCall("BLEDevice::operator bool")
		.andReturnValue(false);

	enterState_Scanning();
	turnTable::tick();
}

TEST(turnTable_test, tick_Scanning_wrongName){
	expect_Scanning_until_peripheral();
	mock().expectOneCall("BLEDevice::localName")
		.andReturnValue("blabla");

	enterState_Scanning();
	turnTable::tick();
}

TEST(turnTable_test, tick_Scanning_connect_fail){
	expect_Scanning_until_stopScan();
	mock().expectOneCall("BLEDevice::connect")
		.andReturnValue(false);

	enterState_Scanning();
	turnTable::tick();
}

TEST(turnTable_test, tick_Scanning_discoverAttributes_fail){
	expect_Scanning_until_connect();
	mock().expectOneCall("BLEDevice::discoverAttributes")
		.andReturnValue(false);
	mock().expectOneCall("BLEDevice::disconnect");

	enterState_Scanning();
	turnTable::tick();
}

TEST(turnTable_test, tick_Scanning_turnCharacteristic_fail){
	expect_Scanning_until_discoverAttributes();
	mock().expectOneCall("BLECharacteristic::operator bool")
		.andReturnValue(false);
	mock().expectOneCall("BLEDevice::disconnect");

	enterState_Scanning();
	turnTable::tick();
}

TEST(turnTable_test, tick_Scanning_canWrite_fail){
	expect_Scanning_until_turnCharacteristic();
	mock().expectOneCall("BLECharacteristic::canWrite")
		.andReturnValue(false);
	mock().expectOneCall("BLEDevice::disconnect");

	enterState_Scanning();
	turnTable::tick();
}

TEST(turnTable_test, tick_Connected_fail){
	enterState_Connected();
	mock().expectOneCall("BLEDevice::connected")
		.andReturnValue(false);
	turnTable::tick(); //lets the state machine change to state StartConnecting

	//check if we are in state StartConnecting now
	mock().expectOneCall("BLELocalDevice::scanForUuid")
		.ignoreOtherParameters();
	turnTable::tick();
}

TEST(turnTable_test, tick_Connected_pass){
	enterState_Connected();
	mock().expectOneCall("BLEDevice::connected")
		.andReturnValue(true);
	turnTable::tick();

	//check if we are still in state Connected now
	mock().expectOneCall("BLEDevice::connected");
	turnTable::tick();
}

TEST(turnTable_test, TurnClockWise){
	mock().expectOneCall("BLECharacteristic::writeValue")
		.withParameter("value", 0x01);

	using namespace turnTable;
	sendCommand(Command::TurnClockWise);
}

TEST(turnTable_test, TurnCounterClockWise){
	mock().expectOneCall("BLECharacteristic::writeValue")
		.withParameter("value", 0x02);

	using namespace turnTable;
	sendCommand(Command::TurnCounterClockWise);
}

TEST(turnTable_test, sendInvalidCommand){
	mock().expectNoCall("BLECharacteristic::writeValue");

	using namespace turnTable;
	sendCommand((Command)123);
}

TEST(turnTable_test, isConnected_false){
	mock().ignoreOtherCalls();
	turnTable::init();
	CHECK_FALSE(turnTable::isConnected());
}

TEST(turnTable_test, isConnected_true){
	enterState_Connected();
	CHECK(turnTable::isConnected());
}

TEST(turnTable_test, disconnect){
	mock().expectOneCall("BLEDevice::disconnect");
	turnTable::disconnect();
}
*/