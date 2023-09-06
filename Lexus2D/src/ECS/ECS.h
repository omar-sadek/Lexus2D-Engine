#ifndef ECS_H
#define ECS_H

#include "../Logger/Logger.h"
#include<bitset>
#include<vector>
#include<unordered_map>
#include<typeindex>
#include <set>
#include <deque>
#include <memory>

enum Tag {
	PLAYER_T = 0,
	OBSTACLE_T = 1
};

enum Group {
	ALLIES_G = 0,
	ENEMIES_G = 1,
	OBSTACLES_G = 2,
	TILES_G = 3,
	PROJECTILES_G = 4
};

const unsigned int MAX_COMPONENTS = 50;

/// <summary>A Signature is used to keep track of which compnents an entity has.</summary>
typedef std::bitset< MAX_COMPONENTS> Signature;

class Entity {
private:
	int id;

	class EntityManager* entityManager;

public:
	Entity(int i_id) : id(i_id) {};
	Entity(const Entity& i_entity) = default;
	void Destroy();
	int GetId() const;

	void SetTag(const Tag& i_tag);
	bool HasTag(const Tag& i_tag) const;
	void SetGroup(const Group& i_group);
	bool IsInGroup(const Group& i_group) const;

	Entity& operator =(const Entity& i_other) = default;
	bool operator ==(const Entity& i_other) const { return id == i_other.GetId(); }
	bool operator !=(const Entity& i_other) const { return id != i_other.GetId(); }
	bool operator >(const Entity& i_other) const { return id > i_other.GetId(); }
	bool operator <(const Entity& i_other) const { return id < i_other.GetId(); }

	void SetEntityManager(EntityManager* i_entityManager);

	template <typename TComponent, typename ...TArgs> void AddComponent(TArgs&& ...i_args);
	template <typename TComponent> void RemoveComponent();
	template <typename TComponent> bool HasComponent() const;
	template <typename TComponent> TComponent& GetComponent() const;
};

struct BaseComponent {
protected:
	static int nextId;
};

template <typename T>
class Component : public BaseComponent {
public:
	/// <summary>Returns a unique id of each Component./summary>
	static int GetId() {
		//this will happen once
		static auto id = nextId++;
		return id;
	}
};

/// <summary>Processes entities that contain a specific signature.</summary>
class System {
private:
	Signature componentSignature;
	std::vector<Entity> entities;
protected:
	std::shared_ptr<EntityManager> entityManager;
public:
	System() = default;
	~System() = default;

	void AddEntityToSystem(Entity i_entity);
	void RemoveEntityFromSystem(Entity i_entity);
	std::vector<Entity> GetSystemEntities() const;
	const Signature& GetComponentSignature() const;

	/// <summary>Defines the component type that entities require to be used by the system.</summary>
	template <typename TComponent> void RequireComponent();
};

class IPool {
public:
	virtual ~IPool() = default;
	virtual void RemoveEntityFromPool(int i_entityId) = 0;
};

template <typename T>
class Pool : public IPool {
private:
	std::vector<T> data;
	int size;

	std::unordered_map<int, int> entityIdToIndex;
	std::unordered_map<int, int> indexToEntityId;

public:
	Pool(int i_capacity = 50) {
		size = 0;
		data.resize(i_capacity);
	}
	virtual ~Pool() = default;

#pragma region Pool Data Utility 

	bool IsEmpty() const {
		return size == 0;
	}

	int GetSize() const {
		return size;
	}

	void Resize(int i_n) {
		data.resize(i_n);
	}

	void Clear() {
		data.clear();
		size = 0;
	}

	void Add(T i_poolElement) {
		data.push_back(i_poolElement);
	}

	void Set(int i_entityId, T i_poolElement) {
		if (entityIdToIndex.find(i_entityId) != entityIdToIndex.end()) {
			int index = entityIdToIndex[i_entityId];
			data[index] = i_poolElement;
		}
		else {
			int index = size;
			entityIdToIndex.emplace(i_entityId, index);
			indexToEntityId.emplace(index, i_entityId);

			if (index >= data.capacity()) {
				data.resize(size * 2);
			}

			data[index] = i_poolElement;
			size++;
		}
	}

	void Remove(int i_entityId) {
		int indexToRemove = entityIdToIndex[i_entityId];
		data[indexToRemove] = data[size - 1];

		int entityOfLast = indexToEntityId[size - 1];
		entityIdToIndex[entityOfLast] = indexToRemove;
		indexToEntityId[indexToRemove] = entityOfLast;

		entityIdToIndex.erase(i_entityId);
		indexToEntityId.erase(size - 1);
		size--;
	}

	void RemoveEntityFromPool(int i_entityId) override {
		if (entityIdToIndex.find(i_entityId) != entityIdToIndex.end()) {
			Remove(i_entityId);
		}
	}

	T& Get(int i_entityId) {
		int index = entityIdToIndex[i_entityId];
		return static_cast<T&>(data[index]);
	}

	T& operator [](unsigned int i_index) {
		return data[i_index];
	}

#pragma endregion
};

class EntityManager {
private:
	int entitiesCount = 0;
	/// <summary>Entities the is going to be created in the next frame.</summary>
	std::set<Entity> entitiesToAdd;
	/// <summary>Entities the is going to be destroyed in the next frame.</summary>
	std::set<Entity> entitiesToDestroy;

	/// <summary>Used to get entitiy with tag</summary>
	std::unordered_map<Tag, Entity> entityPerTag;
	/// <summary>Used to get tag with entity</summary>
	std::unordered_map<int, Tag> tagPerEntity;

	/// <summary>Used to get entities with group</summary>
	std::unordered_map<Group, std::set<Entity>> entitiesPerGroup;
	/// <summary>Used to get group with entity</summary>
	std::unordered_map<int, Group> groupPerEntity;

	/// <summary>Entity ids that were used before getting destroyed, they're stored in deque to reuse them.</summary>
	std::deque<int> freeIds;

	/// <summary>Vector[index] = Component Type ID.</summary>
	std::vector<std::shared_ptr<IPool>> componentPools;
	/// <summary>Vector[index] = Entity ID.</summary>
	std::vector<Signature> entityComponentSignatures;

	std::unordered_map<std::type_index, std::shared_ptr<System>> systems;

public:
	EntityManager() = default;
	void Update();

	// Tag management
	void TagEntity(Entity i_entity, const Tag& i_tag);
	bool EntityHasTag(Entity i_entity, const Tag& i_tag) const;
	Entity GetEntityByTag(const Tag& i_tag) const;
	void RemoveEntityTag(Entity i_entity);

	// Group management
	void GroupEntity(Entity i_entity, const Group& i_group);
	bool IsEntityInGroup(Entity i_entity, const Group& i_group) const;
	std::vector<Entity> GetEntitiesByGroup(const Group& i_group) const;
	void RemoveEntityGroup(Entity i_entity);

	//E
	Entity CreateEntity();
	void DestroyEntity(Entity i_entity);

	//C
	template <typename TComponent, typename ...TArgs> void AddComponent(Entity i_entity, TArgs&& ...i_args);
	template <typename TComponent> void RemoveComponent(Entity i_entity);
	template <typename TComponent> bool HasComponent(Entity i_entity) const;
	template <typename TComponent> TComponent& GetComponent(Entity i_entity) const;

	//S
	template<typename TSystem, typename ...TArgs> void AddSystem(TArgs&& ...i_args);
	template<typename TSystem> void RemoveSystem();
	template<typename TSystem> bool HasSystem() const;
	template<typename TSystem> TSystem& GetSystem() const;

	//ES
	void AddEntityToSystems(Entity i_entity);
	void RemoveEntityFromSystems(Entity i_entity);

};

template <typename TComponent>
void System::RequireComponent() {
	const auto componentId = Component<TComponent>::GetId();
	componentSignature.set(componentId);
}

#pragma region EntityManager Templates

template <typename TComponent, typename ...TArgs>
void EntityManager::AddComponent(Entity i_entity, TArgs&& ...i_args) {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = i_entity.GetId();

	if (componentId >= componentPools.size()) {
		componentPools.resize(componentId + 1, nullptr);
	}

	if (!componentPools[componentId]) {
		std::shared_ptr<Pool<TComponent>> newComponentPool = std::make_shared<Pool<TComponent>>();
		componentPools[componentId] = newComponentPool;
	}

	std::shared_ptr<Pool<TComponent>> componentPool =
		std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);

	TComponent newComponent(std::forward<TArgs>(i_args)...);
	componentPool->Set(entityId, newComponent);
	entityComponentSignatures[entityId].set(componentId);

	//Logger::Log("Component ID: " + std::to_string(componentId) + " was added to Entity ID : " + std::to_string(entityId));
}

template <typename TComponent>
void EntityManager::RemoveComponent(Entity i_entity) {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = i_entity.GetId();

	std::shared_ptr<Pool<TComponent>> componentPool =
		std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
	componentPool->Remove(entityId);

	entityComponentSignatures[entityId].set(componentId, false);

	//Logger::Log("Component ID: " + std::to_string(componentId) + " was removed from Entity ID : " + std::to_string(entityId));
}

template <typename TComponent>
bool EntityManager::HasComponent(Entity i_entity) const {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = i_entity.GetId();

	return entityComponentSignatures[entityId].test(componentId);
}

template <typename TComponent>
TComponent& EntityManager::GetComponent(Entity i_entity) const {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = i_entity.GetId();
	std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
	return componentPool->Get(entityId);
}

template<typename TSystem, typename ...TArgs>
void EntityManager::AddSystem(TArgs&& ...i_args) {
	std::shared_ptr<TSystem> newSystem = std::make_shared<TSystem>(std::forward<TArgs>(i_args)...);
	systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
}

template<typename TSystem>
void EntityManager::RemoveSystem() {
	auto system = systems.find(std::type_index(typeid(TSystem)));
	systems.erase(system);
}

template<typename TSystem>
bool EntityManager::HasSystem() const {
	return systems.find(std::type_index(typeid(TSystem))) != systems.end();
}

template<typename TSystem>
TSystem& EntityManager::GetSystem() const {
	auto system = systems.find(std::type_index(typeid(TSystem)));
	return *(std::static_pointer_cast<TSystem>(system->second));
}

#pragma endregion

#pragma region Entity Templates

template <typename TComponent, typename ...TArgs>
void Entity::AddComponent(TArgs&& ...i_args) {
	entityManager->AddComponent<TComponent>(*this, std::forward<TArgs>(i_args)...);
}

template <typename TComponent>
void Entity::RemoveComponent() {
	entityManager->RemoveComponent<TComponent>(*this);
}

template <typename TComponent>
bool Entity::HasComponent() const {
	return entityManager->HasComponent<TComponent>(*this);
}

template <typename TComponent>
TComponent& Entity::GetComponent() const {
	return entityManager->GetComponent<TComponent>(*this);
}

#pragma endregion
#endif