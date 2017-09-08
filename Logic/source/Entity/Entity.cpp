#include "Entity/Entity.h"

using namespace Logic;

Entity::Entity()
{
}

Entity::~Entity() { }

void Logic::Entity::update(float deltaTime)
{
	updateSpecific(deltaTime);
}

void Entity::collision(const Entity& other)
{
	onCollision(other);
}