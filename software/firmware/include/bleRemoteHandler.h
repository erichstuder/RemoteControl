#pragma once

namespace bleRemoteHandler{
	extern void (*init)();
	extern void (*tick)();
	//extern void (*sendCommand)(Command command);
	//extern bool (*isConnected)();
	//extern void (*disconnect)();
};
