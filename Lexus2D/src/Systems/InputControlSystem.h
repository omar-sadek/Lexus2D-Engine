#ifndef INPUTCONTROLSYSTEM_H
#define INPUTCONTROLSYSTEM_H

#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"
#include "../Components/InputControlledComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/RigidBodyComponent.h"

class InputControlSystem : public System {
public:
	InputControlSystem() {
		RequireComponent<InputControlledComponent>();
		RequireComponent<SpriteComponent>();
		RequireComponent<RigidBodyComponent>();
	}

	void SubscribeToEvent(std::unique_ptr<EventBus>& i_eventManager) {
		i_eventManager->SubscribeToEvent<KeyPressedEvent>(this, &InputControlSystem::OnKeyPressed);
	}

	void OnKeyPressed(KeyPressedEvent& i_event) {
		for (Entity entity : GetSystemEntities()) {
			const InputControlledComponent control = entity.GetComponent<InputControlledComponent>();
			SpriteComponent& sprite = entity.GetComponent<SpriteComponent>();
			RigidBodyComponent& rigidBody = entity.GetComponent<RigidBodyComponent>();

			if (entity.HasComponent<FrameSwapperComponent>()) {

			}

			switch (i_event.KeyCode) {
			case SDLK_UP:
				rigidBody.velocity = control.upVelocity;
				sprite.srcRect.y = sprite.height * 7;
				setAnimationActive(entity, true);
				break;
			case SDLK_RIGHT:
				rigidBody.velocity = control.rightVelocity;
				sprite.srcRect.y = sprite.height * 5;
				setAnimationActive(entity, true);
				break;
			case SDLK_DOWN:
				rigidBody.velocity = control.downVelocity;
				sprite.srcRect.y = sprite.height * 4;
				setAnimationActive(entity, true);
				break;
			case SDLK_LEFT:
				rigidBody.velocity = control.leftVelocity;
				sprite.srcRect.y = sprite.height * 6;
				setAnimationActive(entity, true);
				break;
			case SDLK_SPACE:
				rigidBody.velocity = glm::vec2(0);
				sprite.srcRect.y /= sprite.height;
				int multiplier = sprite.srcRect.y - 4;
				sprite.srcRect.y = sprite.height * multiplier;
				//setAnimationActive(entity, false);
				break;
			}
		}
	}


	void Update() {
		
	}

private:
	void setAnimationActive(Entity i_entity, bool i_active) {
		if (i_entity.HasComponent<FrameSwapperComponent>()) {
			FrameSwapperComponent& animation = i_entity.GetComponent<FrameSwapperComponent>();
			animation.isRunning = i_active;
		}
	}
};

#endif