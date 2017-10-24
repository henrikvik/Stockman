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

	updateSound(deltaTime);

	// Checking different buffs
	for (auto &effectPair : m_statusManager.getActiveEffects()) //opt
		affect(effectPair.first, *effectPair.second, deltaTime);
	
	// Updating buffs duration
	m_statusManager.update(deltaTime);
}

void Entity::upgrade(Upgrade const & upgrade) { }

void Entity::updateSound(float deltaTime)
{
	// Update sound position
	btVector3 pos = getPositionBT();
	btVector3 vel = getRigidBody()->getLinearVelocity();
	m_soundSource.pos = { pos.x(), pos.y(), pos.z() };
	m_soundSource.vel = { vel.x(), vel.y(), vel.z() };
	m_soundSource.update(deltaTime);
}

void Entity::addCallback(Entity::EntityEvent entityEvent, callback callback)
{
    m_callbacks[entityEvent] = callback;
}

bool Entity::hasCallback(EntityEvent entityEvent) const
{
    return m_callbacks.find(entityEvent) != m_callbacks.end();
}

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

std::unordered_map<Entity::EntityEvent, std::function<void (Entity::CallbackData&)>>& Entity::getCallbacks()
{
    return m_callbacks;
}
