#pragma once

namespace bleRemoteHandler{
	enum class Command{
		TurnTable_TurnClockwise,
		TurnTable_TurnCounterClockwise,
		BleUsbKeyboard_SendText,
	};

	extern void (*init)();
	extern void (*tick)();
	//extern bool (*isConnected)();
	extern void (*disconnectAccessories)();
	extern void (*send)(Command command);
};
