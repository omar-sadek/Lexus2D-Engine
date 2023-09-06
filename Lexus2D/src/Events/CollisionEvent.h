#ifndef COLLISIONEVENT_H
#define COLLISIONEVENT_H

#include "../ECS/ECS.h"
#include "../EventBus/Event.h"

class CollisionEvent : public Event {
public:
	Entity A;
	Entity B;
	CollisionEvent(Entity i_a, Entity i_b) : A(i_a), B(i_b) {}
};

#endif