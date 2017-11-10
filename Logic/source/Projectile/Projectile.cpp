#include "../Projectile/Projectile.h"
#include "../Player/Player.h"
#include <AI\Enemy.h>
#include <Graphics\include\Particles\ParticleSystem.h>
#include <Engine\DebugWindow.h>
#include <Physics\Physics.h>

using namespace Logic;

// TEMP: ta bort mig
static bool FUN_MODE = false;

Projectile::Projectile(btRigidBody* body, btVector3 halfextent)
: Entity(body, halfextent, Graphics::ModelID::SPHERE) 
{
	m_pData.damage = 1;
	m_pData.speed = 0.f;
	m_pData.gravityModifier = 1.f;
	m_pData.ttl = 1000.f;
	m_remove = false;
	m_bulletTimeMod = 1.f;
}

Logic::Projectile::Projectile(btRigidBody* body, btVector3 halfExtent, ProjectileData pData)
: Entity(body, halfExtent, Graphics::ModelID::SPHERE)
{
	m_pData = pData;
	m_remove = false;
	setModelID(pData.meshID);
	m_bulletTimeMod = 1.f;

	/*switch (pData.type)
	{
		// Do specifics 
	}*/
}

Projectile::~Projectile() { }

void Projectile::start(btVector3 forward, StatusManager& statusManager)
{
    // TEMP: ta bort mig
    DebugWindow *debugWindow = DebugWindow::getInstance();
    debugWindow->registerCommand("GFX_FUNMODE", [&](std::vector<std::string> &args) -> std::string
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
	auto flags = effect.getStandards()->flags;

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

// This function is a bloody mess
void Projectile::onCollision(PhysicsObject& other, btVector3 contactPoint, float dmgMultiplier)
{
    if (!m_remove)
    {
        // TEMP
        bool callback = false;

        if (Projectile* proj = dynamic_cast<Projectile*> (&other))  // if projectile
        {
            if (proj->getProjectileData().type == ProjectileTypeBulletTimeSensor)
            {
                getStatusManager().addStatus(StatusManager::EFFECT_ID::BULLET_TIME,
                    proj->getStatusManager().getStacksOfEffectFlag(Effect::EFFECT_FLAG::EFFECT_BULLET_TIME), true);
                callback = true;
            }
            else if (proj->getProjectileData().type == ProjectileTypeMeleeParry)
            {
                m_remove = true;
                callback = false;
            }
            else {} // this might seem really pointless, because it is, but if you remove it, it will stop working, so dont touch this godly else
        }
        else if (!dynamic_cast<Player*>(&other)) // if not player
        {
            if (dynamic_cast<Enemy*> (&other) && getProjectileData().enemyBullet)
            {
                m_remove = false;
            }
            else if (!dynamic_cast<Enemy*> (&other) && m_pData.type == ProjectileTypeMeleeParry)
            {
                callback = false;
            }
            else
            {
                m_remove = true;
                callback = true;

                // DELETE THIS 
                if (dynamic_cast<Enemy*> (&other))
                {
                    if (FUN_MODE)
                    {
                        if (m_pData.damage > 0)
                            Graphics::FXSystem->addEffect("IceExplosion", DirectX::XMMatrixTranslationFromVector(getPosition()));

                        // DELETE THIS 
                        if (dynamic_cast<Enemy*> (&other)->getHealth() < 0)
                        {
                            Graphics::FXSystem->addEffect("IceExplosion", DirectX::XMMatrixTranslationFromVector(getPosition() + DirectX::SimpleMath::Vector3(2, 2, 2)));
                            Graphics::FXSystem->addEffect("IceExplosion", DirectX::XMMatrixTranslationFromVector(getPosition() + DirectX::SimpleMath::Vector3(-3, 2, -3)));
                            Graphics::FXSystem->addEffect("IceExplosion", DirectX::XMMatrixTranslationFromVector(getPosition() + DirectX::SimpleMath::Vector3(-2, 3, -3)));
                            Graphics::FXSystem->addEffect("IceExplosion", DirectX::XMMatrixTranslationFromVector(getPosition() + DirectX::SimpleMath::Vector3(3, 2, 3)));
                            Graphics::FXSystem->addEffect("IceExplosion", DirectX::XMMatrixTranslationFromVector(getPosition() + DirectX::SimpleMath::Vector3(2, 3, -2)));
                            Graphics::FXSystem->addEffect("IceExplosion", DirectX::XMMatrixTranslationFromVector(getPosition() + DirectX::SimpleMath::Vector3(0, 6, 0)));
                            Graphics::FXSystem->addEffect("IceExplosion", DirectX::XMMatrixTranslationFromVector(getPosition() + DirectX::SimpleMath::Vector3(0, 0, 0)));
                        }
                    }
                    if (m_pData.type == ProjectileTypeIce)
                    {
                        dynamic_cast<Enemy*> (&other)->getStatusManager().addStatus(StatusManager::FREEZE, 1, true);

                        if (FUN_MODE)
                            if ((rand() % 100) == 1)
                                Graphics::FXSystem->addEffect("IceExplosion", DirectX::XMMatrixTranslationFromVector(getPosition()));
                    }
                }
            }

            for (StatusManager::UPGRADE_ID upgrade : this->getStatusManager().getActiveUpgrades())
                if (this->getStatusManager().getUpgrade(upgrade).getTranferEffects() & Upgrade::UPGRADE_IS_BOUNCING)
                    m_remove = false;
        }
        else if (getProjectileData().enemyBullet)  // if player and enemy bullet
        {
            m_remove = true;
            callback = true;
        }

        if (callback && hasCallback(ON_COLLISION))
        {
            if (m_pData.type == ProjectileTypeMeleeParry)
                int i = 0;
            CallbackData data;
            data.caller = dynamic_cast<Entity*>(this);
            data.dataPtr = reinterpret_cast<std::intptr_t> (&other);
            this->callback(ON_COLLISION, data);
        }

        if (m_pData.type == ProjectileTypeBulletTimeSensor ||
            m_pData.type == ProjectileTypeMelee ||
            m_pData.type == ProjectileTypeMeleeParry ||
            m_pData.type == ProjectileTypeIce)
            m_remove = false;

        // DELETE THIS 
        if (m_remove && FUN_MODE)
            Graphics::FXSystem->addEffect("IceExplosion", DirectX::XMMatrixTranslationFromVector(getPosition()));
    }
}

void Projectile::upgrade(Upgrade const &upgrade)
{
	long long flags = upgrade.getTranferEffects();
    if (flags & Upgrade::UPGRADE_INCREASE_DMG)
    {
        m_pData.damage += upgrade.getFlatUpgrades().increaseDmg;
    }
	if (flags & Upgrade::UPGRADE_IS_BOUNCING)
	{
		this->getRigidBody()->setRestitution(1.f);
	}
}

ProjectileData& Projectile::getProjectileData() { return m_pData; }

void Logic::Projectile::setProjectileData(ProjectileData pData)
{
    m_pData = pData;
}

void Logic::Projectile::toRemove()
{
	m_remove = true;
}

void Logic::Projectile::toRemove(bool remove)
{
    m_remove = remove;
}

bool Logic::Projectile::shouldRemove() const
{
	return m_remove;
}
