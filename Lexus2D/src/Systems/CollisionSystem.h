#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"
#include "../Components/ColliderComponent.h"
#include "../Components/TransformComponent.h"

class CollisionSystem : public System {
public:
	CollisionSystem() {
		RequireComponent<TransformComponent>();
		RequireComponent<BoxColliderComponent>();
	}


	void Update(std::unique_ptr<EventBus>& i_eventManager, SDL_Rect& i_camera) {
		std::vector<Entity> entities = GetSystemEntities();

		//TODO:: Optimize using Space Partitioning instead of Sweep and Prune

		for (Entity entity : entities)
		{
			const TransformComponent transform = entity.GetComponent<TransformComponent>();
			BoxColliderComponent& boxCollider = entity.GetComponent<BoxColliderComponent>();

			boxCollider.rect.x = transform.position.x + boxCollider.offset.x - i_camera.x;
			boxCollider.rect.y = transform.position.y + boxCollider.offset.y - i_camera.y;
			boxCollider.rect.w = transform.scale.x * boxCollider.width;
			boxCollider.rect.h = transform.scale.y * boxCollider.height;
		}

		std::sort(entities.begin(), entities.end(),
			[=](Entity i_aEntity, Entity i_bEntity) -> bool {
				const BoxColliderComponent aCollider = i_aEntity.GetComponent<BoxColliderComponent>();
				const BoxColliderComponent bCollider = i_bEntity.GetComponent<BoxColliderComponent>();
				return aCollider.rect.x < bCollider.rect.x;
			});

		auto collisionPairs = sweepAndPruneAlgo(entities);
		for (int i = 0; i < collisionPairs.size(); ++i) {

			BoxColliderComponent* xBoxCollider = &collisionPairs[i].front().GetComponent<BoxColliderComponent>();
			BoxColliderComponent* yBoxCollider = &collisionPairs[i].back().GetComponent<BoxColliderComponent>();

			if (SDL_HasIntersection(&xBoxCollider->rect, &yBoxCollider->rect)) {
				i_eventManager->DispatchEvent<CollisionEvent>(collisionPairs[i].front(), collisionPairs[i].back());

				xBoxCollider->isTriggered = true;
				yBoxCollider->isTriggered = true;
			}
			else {
				xBoxCollider->isTriggered = false;
				yBoxCollider->isTriggered = false;
			}
		}


		/*for (auto i = entities.begin(); i != entities.end(); i++)
		{
			Entity xEntity = *i;
			const TransformComponent xTransform = xEntity.GetComponent<TransformComponent>();
			BoxColliderComponent& xBoxCollider = xEntity.GetComponent<BoxColliderComponent>();

			xBoxCollider.rect.x = xTransform.position.x + xBoxCollider.offset.x - i_camera.x;
			xBoxCollider.rect.y = xTransform.position.y + xBoxCollider.offset.y - i_camera.y;
			xBoxCollider.rect.w = xTransform.scale.x * xBoxCollider.width;
			xBoxCollider.rect.h = xTransform.scale.y * xBoxCollider.height;

			for (auto j = i + 1; j != entities.end(); j++)
			{
				Entity yEntity = *j;

				const TransformComponent yTransform = yEntity.GetComponent<TransformComponent>();
				BoxColliderComponent& yBoxCollider = yEntity.GetComponent<BoxColliderComponent>();

				yBoxCollider.rect.x = yTransform.position.x + yBoxCollider.offset.x - i_camera.x;
				yBoxCollider.rect.y = yTransform.position.y + yBoxCollider.offset.y - i_camera.y;
				yBoxCollider.rect.w = yTransform.scale.x * yBoxCollider.width;
				yBoxCollider.rect.h = yTransform.scale.y * yBoxCollider.height;


				if (SDL_HasIntersection(&xBoxCollider.rect, &yBoxCollider.rect)) {
					i_eventManager->DispatchEvent<CollisionEvent>(xEntity, yEntity);
					xBoxCollider.isTriggered = true;
					yBoxCollider.isTriggered = true;
				}
				else {
					xBoxCollider.isTriggered = false;
					yBoxCollider.isTriggered = false;
				}
			}
		}*/
	}
private:
	std::vector<std::vector<Entity>> sweepAndPruneAlgo(std::vector<Entity>& i_sortedEntites) {

		std::vector<std::vector<Entity>> allPairs;
		std::vector<Entity> activeList; //temporary list

		for (auto i = 0; i < i_sortedEntites.size(); ++i) {
			BoxColliderComponent iCollider = i_sortedEntites[i].GetComponent<BoxColliderComponent>();
			for (auto j = 0; j < activeList.size(); ++j) {
				BoxColliderComponent jCollider = i_sortedEntites[j].GetComponent<BoxColliderComponent>();
				if (iCollider.rect.x >= (jCollider.rect.x + jCollider.rect.w)) {
					activeList.erase(activeList.begin() + j);
					j--;
				}
				else {
					std::vector<Entity> mpair;
					mpair.push_back(i_sortedEntites[i]);
					mpair.push_back(activeList[j]);
					allPairs.push_back(mpair);
				}
			}
			activeList.push_back(i_sortedEntites[i]);
		}

		return allPairs;
	}
};

#endif