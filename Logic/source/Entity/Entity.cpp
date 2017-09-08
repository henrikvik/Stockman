#include "Entity/Entity.h"

using namespace Logic;

Entity::Entity()
{

}

Entity::~Entity() { }

void Entity::createRigidBody(Physics* physics)
{
	/* TEMP CREATION OF A FALLING SPHERE */
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btCollisionShape* sphere = new btSphereShape(1);
	btScalar mass = 5;

	m_rigidBody = new btRigidBody(mass, groundMotionState, sphere);

	physics->addRigidBody(m_rigidBody);
}

void Entity::update(float deltaTime)
{
	updateSpecific(deltaTime);
}

void Entity::collision(Entity& other)
{
	onCollision(other);
}