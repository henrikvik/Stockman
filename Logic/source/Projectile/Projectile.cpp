#include <Projectile\Projectile.h>
#include <Player\Player.h>
#include <AI\Enemy.h>
#include <AI\Trigger.h>
#include <Physics\Physics.h>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace Logic;

// TEMP: ta bort mig
static bool FUN_MODE = false;

Projectile::Projectile(btRigidBody* body, btVector3 halfextent, ProjectileData pData)
: Entity(body, halfextent) 
{
	m_pData         = pData;
	m_dead          = false;
    m_bulletTimeMod = 1.f;
}

Projectile::~Projectile() { }

// Sends a projectile in a direction and copy over upgrades & effect from statusManager
// \param forward - What direction this projectile should be sent
// \param statusManager - A statusManager filled with potential buffs & upgrades
void Projectile::start(btVector3 forward, StatusManager& statusManager)
{
	getRigidBody()->setLinearVelocity(forward * m_pData.speed);
    getStatusManager().copyUpgradesFrom(statusManager);
}

// How different effects affect projectiles
void Projectile::affect(int stacks, Effect const & effect, float deltaTime)
{
	long long flags = effect.getStandards()->flags;

	if (flags & Effect::EFFECT_BULLET_TIME)
	{
        // Add slowdown on every bullet except itself
        if (m_pData.type != ProjectileTypeBulletTimeSensor)
		    m_bulletTimeMod *= std::pow(effect.getSpecifics()->isBulletTime, stacks);
	}
}

// How different upgrade affect projectiles
void Projectile::upgrade(Upgrade const &upgrade)
{
    long long flags = upgrade.getTranferEffects();

    if (flags & Upgrade::UPGRADE_INCREASE_DMG)
    {
        m_pData.damage *= upgrade.getFlatUpgrades().increaseDmg;
    }
    if (flags & Upgrade::UPGRADE_IS_BOUNCING)
    {
        m_body->setRestitution(1.f);
    }
    if (flags & Upgrade::UPGRADE_BURNING)
    {
        if (m_pData.type == ProjectileTypeNormal)
        {
            m_pData.type = ProjectileTypeFireArrow;
        }
    }
}

// Specific update loop for projectiles
void Projectile::updateSpecific(float deltaTime)
{
	Entity::update(deltaTime);

    // Modify the velocity & gravity with the bullet time modifier
    btRigidBody* body = getRigidBody();
    btVector3 dir = body->getLinearVelocity().normalized();
    body->setLinearVelocity(dir * m_pData.speed * m_bulletTimeMod);
    body->setGravity({ 0, -PHYSICS_GRAVITY * m_pData.gravityModifier * m_bulletTimeMod, 0.f });

    // Taking the forward vector and getting the pitch and yaw from it
    float pitch = asin(-dir.getY()) - M_PI * 0.5f;
    float yaw = atan2(dir.getX(), dir.getZ());
    //float roll = RandomGenerator::singleton().getRandomFloat(0.f, 2.f * M_PI); // Random roll rotation
    body->getWorldTransform().setRotation(btQuaternion(yaw, pitch - float(180 * M_PI / 180), 0));
    
    // Decrease the lifetime of this bullet
    m_pData.ttl -= deltaTime * m_bulletTimeMod;

    // Reset the bullet time modifier back to normal
    m_bulletTimeMod = 1.f;

    // Updating transform matrix
    renderInfo.transform = getTransformMatrix();
}

// Handle collisions with different types of classes
void Projectile::onCollision(PhysicsObject& other, btVector3 contactPoint, float dmgMultiplier)
{
    bool cb = false;

    if (Enemy* enemy = dynamic_cast<Enemy*> (&other))
    {
        cb = collisionWithEnemy(enemy);
    }
    else if (Projectile* proj = dynamic_cast<Projectile*> (&other))
    {
        cb = collisionWithProjectile(proj);
    }
    else if (Player* player = dynamic_cast<Player*> (&other))
    {
        cb = collisionWithPlayer(player);
    }
    else if (Trigger* trigger = dynamic_cast<Trigger*> (&other))
    {
        cb = collisionWithTrigger(trigger);
    }
    else
    {
        cb = collisionWithTerrain();
    }

    // Send back data to listener
    if (cb) doCallBack(other);
}

// Call back to listener with a ptr to the collided physics object
void Projectile::doCallBack(PhysicsObject& other)
{
    if (hasCallback(ON_COLLISION))
    {
        CallbackData data;
        data.caller = this;
        data.dataPtr = reinterpret_cast<std::intptr_t> (&other);
        this->callback(ON_COLLISION, data);
    }
}

// Projectile-Enemy Collisions, returns true if callback should be activated
bool Projectile::collisionWithEnemy(Enemy* enemy)
{
    bool callback = false;
    bool kill = false;

    if (m_pData.enemyBullet) return false; // no enemy <--> enemy pj coll

    switch (m_pData.type)
    {
    // Special effects
    case ProjectileTypeIce:

        enemy->getStatusManager().addStatus(
            /* Adding Freeze effect */          StatusManager::FREEZE,
            /* Number of stacks */              1,
            /* Reset Duration */                true
        );
        break;

    // These should not get removed on enemy contact
    case ProjectileTypeBulletTimeSensor:
    case ProjectileTypeMelee:
        break;

    case ProjectileTypeFireArrow:
        enemy->getStatusManager().addStatus(
            /* Adding Fire effect */            StatusManager::ON_FIRE,
            /* Number of stacks */              1,
            /* Don't reset Duration */          false
        );
        break;
    // Trigger all callbacks on other projectiles
    //  And kill them off
    default: 
        callback = true;
        kill = true;
    }
    // Set if we should kill it
    m_dead = kill;

    // Always trigger callback
    return callback;
}

// Projectile Trigger Collisions, returns true if callback should be activated
bool Projectile::collisionWithTrigger(Trigger* trigger)
{
    bool cb = false;

    switch (m_pData.type)
    {
    case ProjectileTypeGrappling:
        m_dead = true;
        cb = true;
    }

    return cb;
}

// Projectile-Terrain Collisions, returns true if callback should be activated
bool Projectile::collisionWithTerrain()
{
    m_dead = true;

    // Don't remove if sensor
    if (m_pData.isSensor)
        m_dead = false;

    // Don't remove if bouncing upgraded
    if (getStatusManager().isOwningUpgrade(Upgrade::UPGRADE_FLAG::UPGRADE_IS_BOUNCING))
        m_dead = false;

    // Always trigger callback
    return true;
}

// Projectile-Player Collisions, returns true if callback should be activated
bool Projectile::collisionWithPlayer(Player* player)
{
    // Nothing we have use of at this point

    // Always callback
    return true;
}

// Projectile-Projectile Collisions, returns true if callback should be activated
bool Projectile::collisionWithProjectile(Projectile* proj)
{
    bool callback = false;

	switch (proj->getProjectileData().type)
	{
	case ProjectileTypeBulletTimeSensor:
	    getStatusManager().addStatus(
		/* Adding Bullet time effect */     StatusManager::BULLET_TIME,
		/* Number of stacks */              proj->getStatusManager().getStacksOfEffectFlag(Effect::EFFECT_FLAG::EFFECT_BULLET_TIME),
		/* Reset Duration */                true
	    );
	    break;
	}
    
    // No callback should be added
    return false;
}

void Logic::Projectile::setWorldTransform(DirectX::SimpleMath::Matrix & worldTransform)
{
    renderInfo.transform = worldTransform;
}

void Logic::Projectile::setModelID(Resources::Models::Files modelId)
{
    renderInfo.model = modelId;
}

void Logic::Projectile::render() const
{
    QueueRender(renderInfo);
}

ProjectileData& Projectile::getProjectileData()             { return m_pData;   }
bool Projectile::getDead() const                            { return m_dead;    }
void Projectile::setProjectileData(ProjectileData pData)    { m_pData = pData;  }
void Projectile::setDead(bool dead)                         { m_dead = dead;    }
