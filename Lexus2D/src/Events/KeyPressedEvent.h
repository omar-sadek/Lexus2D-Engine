#ifndef KEYPRESSEDEVENT_H
#define KEYPRESSEDEVENT_H

#include<SDL.h>

#include "../ECS/ECS.h"
#include "../EventBus/Event.h"

class KeyPressedEvent : public Event {
public:
	SDL_Keycode KeyCode;
	KeyPressedEvent(SDL_Keycode i_keyCode) : KeyCode(i_keyCode) {}
};


#endif