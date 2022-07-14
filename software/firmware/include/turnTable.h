#pragma once

#include "BleAccessory.h"

class TurnTable : public BleAccessory{
public:
	enum class Command{
		TurnClockwise,
		TurnCounterClockwise,	
	};
	String getLocalName();
	String getServiceUuid();
	String getCharacteristicUuid();
	void send(Command command);
};
