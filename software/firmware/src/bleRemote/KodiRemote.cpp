#include "KodiRemote.h"

void KodiRemote::send(Command command){
	switch(command){
		case KodiRemote::Command::Up:
			sendText("Todo: insert UpKey");
			break;
		default:
			break;
	}
}
