#include "../../include/Entity/StaticObject.h"

using namespace Logic;

StaticObject::StaticObject(Graphics::ModelID modelID, btRigidBody * body, btVector3 halfExtent)
	: PhysicsObject(body, halfExtent, modelID)
{

}

StaticObject::~StaticObject() { }

void StaticObject::onCollision(PhysicsObject& other, btVector3 contactPoint, float dmgMultiplier) 
{

}

void StaticObject::update(float deltaTime) 
{
	PhysicsObject::updatePhysics(deltaTime);
}