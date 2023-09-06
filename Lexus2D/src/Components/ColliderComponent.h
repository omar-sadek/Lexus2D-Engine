#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include <glm/glm.hpp>
#include <SDL.h>

struct ColliderComponent {
	bool isTriggered;
};

struct BoxColliderComponent : ColliderComponent {
	float width;
	float height;
	glm::vec2 offset;
	SDL_Rect rect;

	BoxColliderComponent(float i_width = 0.0, float i_height = 0.0, glm::vec2 i_offset = glm::vec2(0)) : ColliderComponent() {
		width = i_width;
		height = i_height;
		offset = i_offset; 
	}
};


struct CircularColliderComponent : ColliderComponent {
	float radius;

	CircularColliderComponent(float i_radius = 0.0) : ColliderComponent() {
		radius = i_radius;
	}
};
#endif