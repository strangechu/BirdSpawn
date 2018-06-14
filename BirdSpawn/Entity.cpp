#include "Entity.h"

Entity::Entity()
{
	pos = Vector3(0, 0, 0);
}

Entity::Entity(float a, float b, float c)
{
	pos = Vector3(a, b, c);
}


Entity::~Entity()
{
}

EntityManager* EntityManager::m_instance = nullptr;

EntityManager::EntityManager()
{
	init();
}


EntityManager::~EntityManager()
{
}

void EntityManager::init()
{
	clear();
}

void EntityManager::clear()
{
	entities.clear();
}

void EntityManager::addEntity(Entity entity)
{
	entities.push_back(entity);
}

std::vector<Entity> EntityManager::getAllEntity()
{
	return entities;
}