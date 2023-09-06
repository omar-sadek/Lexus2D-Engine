#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include <glm/glm.hpp>

struct RigidBodyComponent {
	glm::vec2 velocity;
	double rotationSpeed;

	RigidBodyComponent(glm::vec2 i_velocity = glm::vec2(0.0, 0.0), double i_rotationSpeed = 0.0) {
		velocity = i_velocity;
		rotationSpeed = i_rotationSpeed;
	}
};

#endif