#include "../Projectile/Projectile.h"
#include "../Player/Player.h"
#include <AI\Enemy.h>
#include <Graphics\include\Particles\ParticleSystem.h>
#include <Engine\DebugWindow.h>

using namespace Logic;

// TEMP: ta bort mig
static bool FUN_MODE = false;

Projectile::Projectile(btRigidBody* body, btVector3 halfextent)
: Entity(body, halfextent) 
{
	m_pData.damage = 1.f;
	m_pData.speed = 0.f;
	m_pData.gravityModifier = 1.f;
	m_pData.ttl = 1000.f;
	m_remove = false;
	m_bulletTimeMod = 1.f;
}

Projectile::Projectile(btRigidBody* body, btVector3 halfExtent, float damage, float speed, float gravityModifer, float ttl)
: Entity(body, halfExtent)
{
	m_pData.damage = damage;
	m_pData.speed = speed;
	m_pData.gravityModifier = gravityModifer;
	m_pData.ttl = ttl;
	m_remove = false;
	m_bulletTimeMod = 1.f;
}

Logic::Projectile::Projectile(btRigidBody* body, btVector3 halfExtent, ProjectileData pData)
: Entity(body, halfExtent)
{
	m_pData = pData;
	m_remove = false;
	setModelID(pData.meshID);
	m_bulletTimeMod = 1.f;

	switch (pData.type)
	{
		// Do specifics 
	}
}

Projectile::~Projectile() { }

void Projectile::start(btVector3 forward, StatusManager& statusManager)
{
    // TEMP: ta bort mig
    DebugWindow *debugWindow = DebugWindow::getInstance();
    debugWindow->registerCommand("FUNMODE", [&](std::vector<std::string> &args) -> std::string
    {
        FUN_MODE = !FUN_MODE;

        return "Fun mode toggled!";
    });

	getRigidBody()->setLinearVelocity(forward * m_pData.speed);
	setStatusManager(statusManager);

	for (StatusManager::UPGRADE_ID id : statusManager.getActiveUpgrades())
		upgrade(statusManager.getUpgrade(id));
}

void Projectile::affect(int stacks, Effect const & effect, float deltaTime)
{
	int flags = effect.getStandards()->flags;

	if (flags & Effect::EFFECT_BULLET_TIME)
	{
		m_bulletTimeMod *= std::pow(effect.getSpecifics()->isBulletTime, stacks);
	}
}

void Projectile::updateSpecific(float deltaTime)
{
	Entity::update(deltaTime);

	btVector3 vel = getRigidBody()->getLinearVelocity().normalized();
	getRigidBody()->setLinearVelocity(vel * m_pData.speed * m_bulletTimeMod);
	getRigidBody()->setGravity({ 0, -PHYSICS_GRAVITY * m_pData.gravityModifier * m_bulletTimeMod, 0.f });

	if (m_pData.type == ProjectileTypeBulletTimeSensor)
		m_pData.ttl -= deltaTime;
	else
		m_pData.ttl -= deltaTime * m_bulletTimeMod;
	m_bulletTimeMod = 1.f;
}

void Projectile::onCollision(PhysicsObject& other, btVector3 contactPoint, float dmgMultiplier)
{
    if (hasCallback(ON_COLLISION))
    {
        if (Entity *entity = dynamic_cast<Entity*> (&other))
        {
            CallbackData data;
            data.dataPtr = reinterpret_cast<std::intptr_t> (entity);
            getCallbacks()[ON_COLLISION](data);
        }
    }

	// TEMP
	Player* p = dynamic_cast<Player*>(&other);
	Projectile* proj = dynamic_cast<Projectile*> (&other);

	if (proj)
	{
		if(proj->getProjectileData().type == ProjectileTypeBulletTimeSensor)
			getStatusManager().addStatus(StatusManager::EFFECT_ID::BULLET_TIME, proj->getStatusManager().getStacksOfEffectFlag(Effect::EFFECT_FLAG::EFFECT_BULLET_TIME), true);
	}
	else if(!p)
	{
		m_remove = true;

        // TEMP: ta bort mig
        if (FUN_MODE)
            Graphics::FXSystem->addEffect("IceExplosion", DirectX::XMMatrixTranslationFromVector(getPosition()));

		for (StatusManager::UPGRADE_ID upgrade : this->getStatusManager().getActiveUpgrades())
			if (this->getStatusManager().getUpgrade(upgrade).getTranferEffects() & Upgrade::UPGRADE_IS_BOUNCING)
				m_remove = false;
	}

	if (m_pData.type == ProjectileTypeBulletTimeSensor  ||
        m_pData.type == ProjectileTypeIce               ||
        m_pData.type == ProjectileTypeMelee)
		m_remove = false;
}

void Projectile::upgrade(Upgrade const &upgrade)
{
	long long flags = upgrade.getTranferEffects();

	if (flags & Upgrade::UPGRADE_INCREASE_DMG)
	{
		m_pData.damage = upgrade.getFlatUpgrades().increaseDmg;
	}
	if (flags & Upgrade::UPGRADE_IS_BOUNCING)
	{
		this->getRigidBody()->setRestitution(1.f);
	}
}

ProjectileData& Projectile::getProjectileData() { return m_pData; }

void Logic::Projectile::toRemove()
{
	m_remove = true;
}

bool Logic::Projectile::shouldRemove() const
{
	return m_remove;
}
