#ifndef PROJECTILESPAWNERCOMPONENT_H
#define PROJECTILESPAWNERCOMPONENT_H

#include <SDL.h>
#include <glm/glm.hpp>

struct ProjectileSpawnerComponent {
	double speed;
	glm::vec2 direction;
	int frequency;
	int duration;
	int damage;
	bool isFriendly;
	bool isAutomatic;
	int lastSpawnedTime;

	ProjectileSpawnerComponent(double i_speed = 0, glm::vec2 i_direction = glm::vec2(1,0), int i_frequency = 1000, int i_duration = 5000, int i_damage = 10, bool i_isAutomatic = true, bool i_isFriendly = false) {
		speed = i_speed;
		direction = i_direction;
		frequency = i_frequency;
		duration = i_duration;
		damage = i_damage;
		isFriendly = i_isFriendly;
		isAutomatic = i_isAutomatic;
		lastSpawnedTime = SDL_GetTicks();
	}
};

#endif