#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include <glm/glm.hpp>

struct TransformComponent {
	glm::vec2 position;
	glm::vec2 scale;
	double rotation;

	TransformComponent(
		glm::vec2 i_position = glm::vec2(0,0), 
		glm::vec2 i_scale = glm::vec2(1,1), 
		double i_rotation = 0.0) {
		position = i_position;
		scale = i_scale;
		rotation = i_rotation;
	}
};
#endif