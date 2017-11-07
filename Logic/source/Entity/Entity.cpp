#include <Entity/Entity.h>
#include <AI\Enemy.h>
#include <Projectile\ProjectileStruct.h>
#include <Misc\Sound\SoundSource.h>

using namespace Logic;

Entity::Entity(btRigidBody* body, btVector3 halfextent)
: PhysicsObject(body, halfextent)
{
    m_soundSource = new Sound::SoundSource();
}

Entity::~Entity() 
{
    callback(EntityEvent::ON_DESTROY, CallbackData{this});
    delete m_soundSource;
}

void Entity::setSpawnFunctions(std::function<Projectile*(ProjectileData& pData,
    btVector3 position, btVector3 forward, Entity& shooter)> spawnProjectile,
    std::function<Enemy*(ENEMY_TYPE type, btVector3 &pos,
        std::vector<int> effects)> spawnEnemy,
    std::function<Trigger*(int id, btVector3 const &pos,
        std::vector<int> &effects)> spawnTrigger)
{
    SpawnProjectile = spawnProjectile;
    SpawnEnemy = spawnEnemy;
    SpawnTrigger = spawnTrigger;
}

void Entity::clear() { }

void Entity::update(float deltaTime)
{
	PhysicsObject::updatePhysics(deltaTime);

    // Updating positions of sound information
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
	m_soundSource->pos = { pos.x(), pos.y(), pos.z() };
	m_soundSource->vel = { vel.x(), vel.y(), vel.z() };
	m_soundSource->update(deltaTime);
}

void Entity::addCallback(Entity::EntityEvent entityEvent, Callback callback)
{
    m_callbacks[entityEvent].push_back(callback);
}

bool Entity::hasCallback(EntityEvent entityEvent) const
{
    return m_callbacks.find(entityEvent) != m_callbacks.end();
}

void Entity::clearCallbacks(bool callOnDestroy)
{
    if (callOnDestroy)
        callback(ON_DESTROY, CallbackData{ this });
    m_callbacks.clear();
}

void Entity::callback(EntityEvent entityEvent, CallbackData &data)
{
    try
    {
        if (hasCallback(entityEvent))
            for (Callback &callback : m_callbacks[entityEvent])
                callback(data);
    }
    catch (std::exception ex)
    {
        printf("Callback error (Probably null callback data) \n%s\n", ex.what());
    }
}

StatusManager& Entity::getStatusManager()
{
	return m_statusManager;
}

void Entity::setStatusManager(StatusManager & statusManager)
{
	m_statusManager = statusManager;
}

Sound::SoundSource* Entity::getSoundSource()
{
	return m_soundSource;
}