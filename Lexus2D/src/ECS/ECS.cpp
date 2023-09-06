#include "ECS.h"

int BaseComponent::nextId = 0;

void Entity::Destroy() {
	entityManager->DestroyEntity(*this);
}

int Entity::GetId() const {
	return id;
}

void Entity::SetEntityManager(EntityManager* i_entityManager)
{
	entityManager = i_entityManager;
}

void System::AddEntityToSystem(Entity i_entity) {
	entities.push_back(i_entity);
}

void System::RemoveEntityFromSystem(Entity i_entity) {
	entities.erase(
		std::remove_if(entities.begin(), entities.end(),
			[&i_entity](Entity other) {
				return i_entity == other;
			}), entities.end());
}

std::vector<Entity> System::GetSystemEntities() const {
	return entities;
}

const Signature& System::GetComponentSignature() const {
	return componentSignature;
}

Entity EntityManager::CreateEntity() {
	int entityId;

	if (freeIds.empty()) {
		entityId = entitiesCount++;
		if (entityId >= entityComponentSignatures.size()) {
			entityComponentSignatures.resize(entityId + 1);
		}
	}
	else {
		entityId = freeIds.front();
		freeIds.pop_front();
	}

	Entity entity(entityId);
	entity.SetEntityManager(this);
	entitiesToAdd.insert(entity);

	//Logger::Log("Entity created with id: " + std::to_string(entityId));

	return entity;
}

void Entity::SetTag(const Tag& i_tag) {
	entityManager->TagEntity(*this, i_tag);
}

bool Entity::HasTag(const Tag& i_tag) const {
	return entityManager->EntityHasTag(*this, i_tag);
}

void Entity::SetGroup(const Group& i_group) {
	entityManager->GroupEntity(*this, i_group);
}

bool Entity::IsInGroup(const Group& i_group) const {
	return entityManager->IsEntityInGroup(*this, i_group);
}

void EntityManager::DestroyEntity(Entity i_entity) {
	entitiesToDestroy.insert(i_entity);
}

void EntityManager::AddEntityToSystems(Entity i_entity) {
	const auto entityId = i_entity.GetId();
	const Signature entityComponentSignature = entityComponentSignatures[entityId];

	for (auto& system : systems) {
		const auto& systemComponentSignature = system.second->GetComponentSignature();
		bool isMatched = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;

		if (isMatched)
			system.second->AddEntityToSystem(i_entity);
	}
}

void EntityManager::RemoveEntityFromSystems(Entity i_entity) {
	for (auto system : systems) {
		system.second->RemoveEntityFromSystem(i_entity);
	}
}

void EntityManager::TagEntity(Entity i_entity, const Tag& i_tag) {
	entityPerTag.emplace(i_tag, i_entity);
	tagPerEntity.emplace(i_entity.GetId(), i_tag);
}

bool EntityManager::EntityHasTag(Entity i_entity, const Tag& i_tag) const {
	if (tagPerEntity.find(i_entity.GetId()) == tagPerEntity.end()) {
		return false;
	}
	return entityPerTag.find(i_tag)->second == i_entity;
}

Entity EntityManager::GetEntityByTag(const Tag& i_tag) const {
	return entityPerTag.at(i_tag);
}

void EntityManager::RemoveEntityTag(Entity i_entity) {
	auto taggedEntity = tagPerEntity.find(i_entity.GetId());
	if (taggedEntity != tagPerEntity.end()) {
		auto tag = taggedEntity->second;
		entityPerTag.erase(tag);
		tagPerEntity.erase(taggedEntity);
	}
}

void EntityManager::GroupEntity(Entity i_entity, const Group& i_group) {
	entitiesPerGroup.emplace(i_group, std::set<Entity>());
	entitiesPerGroup[i_group].emplace(i_entity);
	groupPerEntity.emplace(i_entity.GetId(), i_group);
}

bool EntityManager::IsEntityInGroup(Entity i_entity, const Group& i_group) const {
	if (entitiesPerGroup.find(i_group) == entitiesPerGroup.end())
		return false;
	auto groupEntities = entitiesPerGroup.at(i_group);
	return groupEntities.find(i_entity.GetId()) != groupEntities.end();
}

std::vector<Entity> EntityManager::GetEntitiesByGroup(const Group& i_group) const {
	auto& setOfEntities = entitiesPerGroup.at(i_group);
	return std::vector<Entity>(setOfEntities.begin(), setOfEntities.end());
}

void EntityManager::RemoveEntityGroup(Entity i_entity) {
	auto groupedEntity = groupPerEntity.find(i_entity.GetId());
	if (groupedEntity != groupPerEntity.end()) {
		auto group = entitiesPerGroup.find(groupedEntity->second);
		if (group != entitiesPerGroup.end()) {
			auto entityInGroup = group->second.find(i_entity);
			if (entityInGroup != group->second.end()) {
				group->second.erase(entityInGroup);
			}
		}
		groupPerEntity.erase(groupedEntity);
	}
}

void EntityManager::Update() {
	for (Entity entity : entitiesToAdd) {
		AddEntityToSystems(entity);
	}
	entitiesToAdd.clear();

	for (Entity entity : entitiesToDestroy) {
		RemoveEntityFromSystems(entity);
		entityComponentSignatures[entity.GetId()].reset();

		for (std::shared_ptr<IPool> pool : componentPools) {
			if(pool)
				pool->RemoveEntityFromPool(entity.GetId());
		}

		freeIds.push_back(entity.GetId());

		RemoveEntityTag(entity);
		RemoveEntityGroup(entity);
	}
	entitiesToDestroy.clear();
}