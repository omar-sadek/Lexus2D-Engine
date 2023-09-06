#ifndef PROJECTILECOMPONENT_H
#define PROJECTILECOMPONENT_H

#include <SDL.h>

struct ProjectileComponent {
	int startTime;
	int duration;
	int damage;
	bool isFriendly;

	ProjectileComponent(int i_duration = 0, int i_damage = 0, bool i_isFriendly = false) {
		startTime = SDL_GetTicks();
		duration = i_duration;
		damage = i_damage;
		isFriendly = i_isFriendly;
	}
};

#endif