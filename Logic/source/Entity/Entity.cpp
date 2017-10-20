#include <Entity/Entity.h>

using namespace Logic;

Entity::Entity(btRigidBody* body, btVector3 halfextent, Graphics::ModelID modelID)
: PhysicsObject(body, halfextent, modelID)
{
}

Entity::~Entity() { }

void Entity::clear() { }

void Entity::update(float deltaTime)
{
	PhysicsObject::updatePhysics(deltaTime);

	// Update sound position
	btVector3 pos = getPositionBT();
	btVector3 vel = getRigidBody()->getLinearVelocity();
	m_soundSource.pos = { pos.x(), pos.y(), pos.z() };
	m_soundSource.vel = { vel.x(), vel.y(), vel.z() };
	m_soundSource.update(deltaTime);

	// Checking different buffs
	for (auto &effectPair : m_statusManager.getActiveEffects()) //opt
		affect(effectPair.first, *effectPair.second, deltaTime);
	
	// Updating buffs duration
	m_statusManager.update(deltaTime);
}

void Entity::upgrade(Upgrade const & upgrade) { }

StatusManager& Entity::getStatusManager()
{
	return m_statusManager;
}

void Entity::setStatusManager(StatusManager & statusManager)
{
	m_statusManager = statusManager;
}

SoundSource* Entity::getSoundSource()
{
	return &m_soundSource;
}