#include "Entity/Entity.h"

using namespace Logic;

Entity::Entity()
{

}

Entity::~Entity() { }

void Entity::collision(const Entity& other)
{
	onCollision(other);
}