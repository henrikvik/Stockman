#include <Entity/Entity.h>
#include <AI\Enemy.h>
#include <Projectile\ProjectileStruct.h>
#include <Projectile\Projectile.h>
#include <Misc\Sound\SoundSource.h>

using namespace Logic;

Entity::Entity(btRigidBody* body, btVector3 halfextent, btVector3 modelOffset)
: PhysicsObject(body, halfextent, modelOffset)
{
    m_soundSource = newd Sound::SoundSource();
}

Entity::~Entity() 
{
    callback(EntityEvent::ON_DESTROY, CallbackData{this});
    delete m_soundSource;
}

void Entity::setSpawnFunctions(ProjectileFunc spawnProjectile, EnemyFunc spawnEnemy, TriggerFunc spawnTrigger)
{
    SpawnProjectile = spawnProjectile;
    SpawnEnemy = spawnEnemy;
    SpawnTrigger = spawnTrigger;
}

void Logic::Entity::SpawnDamageText(int damage, const DirectX::XMVECTORF32 color)
{
    auto scale = damage / 50.f;
    scale = max(0.25, scale);
    scale = min(2.f, scale);

    ProjectileData data = {};
    data.mass = 1.f;
    data.scale = 1.f;
    data.speed = 3.f;
    data.gravityModifier = .3f;
    data.ttl = 2000;
    data.lightInfo.range = 0.f;

    data.fancy.position = getPosition();
    data.fancy.scale = 1.f;
    data.fancy.color = color;
    data.fancy.text = std::to_wstring(damage);
    data.enemyBullet = true;
    data.shouldRender = false;

    auto p = SpawnProjectile(data, getPositionBT(), { RandomGenerator::singleton().getRandomFloat(-0.5f, .5f), 1, RandomGenerator::singleton().getRandomFloat(-0.5f, .5f) }, *this);
    p->getStatusManager().addUpgrade(StatusManager::BOUNCE);
    p->getRigidBody()->setRestitution(10.f);
}

void Entity::clear() { }

void Entity::update(float deltaTime)
{
    PhysicsObject::updatePhysics(deltaTime);
    updateSound(deltaTime);
    m_statusManager.update(deltaTime, *this);
}

void Entity::upgrade(StatusManager::UPGRADE_ID id, int stacks) {
    onUpgradeAdd(stacks, getStatusManager().getUpgrade(id));
    for (int i = 0; i < stacks; i++)
        getStatusManager().addUpgrade(id);
}

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
};

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
        printf("Callback error: %s\n", ex.what());
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