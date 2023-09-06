#ifndef INPUTCONTROLLEDCOMPONENT_H
#define INPUTCONTROLLEDCOMPONENT_H

#include <glm/glm.hpp>

struct InputControlledComponent {
	glm::vec2 upVelocity;
	glm::vec2 rightVelocity;
	glm::vec2 downVelocity;
	glm::vec2 leftVelocity;

	InputControlledComponent(glm::vec2 i_up = glm::vec2(0), glm::vec2 i_right = glm::vec2(0), glm::vec2 i_down = glm::vec2(0), glm::vec2 i_left = glm::vec2(0)) {
		upVelocity = i_up;
		rightVelocity = i_right;
		downVelocity = i_down;
		leftVelocity = i_left;
	}
};

#endif