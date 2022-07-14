#pragma once

namespace bleRemoteHandler{
	enum class Command{
		TurnTable_TurnClockwise,
		TurnTable_TurnCounterClockwise,
		Kodi_Up,
	};

	extern void (*init)();
	extern void (*tick)();
	extern void (*disconnectAllAccessories)();
	extern void (*send)(Command command);
};
