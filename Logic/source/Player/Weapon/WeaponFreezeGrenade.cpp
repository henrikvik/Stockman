#include <Player\Weapon\WeaponFreezeGrenade.h>
#include <Projectile\Projectile.h>
#include <Projectile\ProjectileStruct.h>
#include <Misc\Sound\NoiseMachine.h>
#include <AI\Enemy.h>
#include <Entity\Entity.h>

#define _USE_MATH_DEFINES
#include <math.h>


using namespace Logic;

#define FREEZE_GRENADE_SPEED_LOW                8.f
#define FREEZE_GRENADE_SPEED_HIGH               32.f
#define FREEZE_GRENADE_SPLIT_DIR_LOW            0.8f
#define FREEZE_GRENADE_SPLIT_DIR_HIGH           1.2f
#define FREEZE_GRENADE_SPLIT_UPDIR_LOW          3.f
#define FREEZE_GRENADE_SPLIT_UPDIR_HIGH         4.f
#define FREEZE_GRENADE_EXPLOSION_SCALE          { 8.75f, 8.75f, 8.75f }
#define FREEZE_GRENADE_SPLIT_EXPLOSION_SCALE    { 5.f, 5.f, 5.f }
#define FREEZE_GRENADE_DMG                      50
#define FREEZE_GRENADE_DMG_ENHANCED             100

WeaponFreezeGrenade::WeaponFreezeGrenade()
{
}

WeaponFreezeGrenade::WeaponFreezeGrenade(ProjectileManager* projectileManager, ProjectileData& projectileData, WeaponInfo wInfo, ProjectileData& freezeData, ProjectileData& explosionData, int splitCount)
    : Weapon(projectileManager, projectileData, wInfo)
{
    m_freezeData = freezeData;
    m_splitCount = splitCount;
    m_explosionData = explosionData;
    m_sliceSize = (2 * M_PI) / m_splitCount;
    m_enhanced = false;
}


WeaponFreezeGrenade::~WeaponFreezeGrenade()
{

}

void WeaponFreezeGrenade::onUse(std::vector<Projectile*> &projectiles, Entity& shooter)
{
    shooter.getSoundSource()->playSFX(Sound::SFX::WEAPON_MAGIC_1, 1.f, 0.05f);

    if (m_enhanced)
    {
        for (Projectile* p : projectiles)
        {
            p->addCallback(Entity::ON_DESTROY, [&](Entity::CallbackData &data) -> void {
                doExplosionCallback(data);
                doExplosionCallbackEnhanced(data);
            });
        }
    }
    else
    {
        for (Projectile* p : projectiles)
        {
            p->addCallback(Entity::ON_DESTROY, [&](Entity::CallbackData &data) -> void {
                doExplosionCallback(data);
            });
        }
    }
    
}

void WeaponFreezeGrenade::doExplosionCallback(Entity::CallbackData &data)
{
    data.caller->getSoundSource()->playSFX(Sound::SFX::WEAPON_MAGIC_2, 1.f, 0.15f);
    
    // explosion projectile
    m_explosionData.hitboxScale = FREEZE_GRENADE_EXPLOSION_SCALE;
    Projectile* explosion = getSpawnProjectileFunc()(m_explosionData, data.caller->getPositionBT(), { 0.f, 0.f, 0.f }, *data.caller, { 0.f, 0.f, 0.f });

    if (explosion)
    {
        // Graphical explosion - particle
        Graphics::FXSystem->addEffect("IceExplosion", data.caller->getPosition());

        // callback
        explosion->addCallback(Entity::ON_COLLISION, [&](Entity::CallbackData &data) -> void {

            data.caller->getSoundSource()->playSFX(Sound::SFX::WEAPON_ICEGUN_THIRD, 1.f, 0.33f);
            
            if (Projectile* p = dynamic_cast<Projectile*>(data.caller))
                p->getProjectileData().type = ProjectileTypeNormal;

            PhysicsObject* obj = reinterpret_cast<PhysicsObject*>(data.dataPtr);

            if (Enemy* enemy = dynamic_cast<Enemy*>(obj))
            {
                btVector3 knockbackDir = (obj->getPositionBT() - data.caller->getPositionBT()).normalize();
                obj->getRigidBody()->setLinearVelocity(knockbackDir * 8.f);
                enemy->getStatusManager().addStatusResetDuration(StatusManager::EFFECT_ID::STUN, 1);
            }
        });
    }
}

void WeaponFreezeGrenade::doExplosionCallbackEnhanced(Entity::CallbackData &data)
{
    // cascade
    PhysicsObject* obj = reinterpret_cast<PhysicsObject*>(data.dataPtr);

    btVector3 position;

    if (dynamic_cast<Entity*>(obj))
        position = data.caller->getPositionBT();
    else
        position = data.caller->getPositionBT() + btVector3(0.f, 1.f, 0.f);

    float dirMod;
    float upMod;
    RandomGenerator &rng = RandomGenerator::singleton();

    for (int i = 0; i < m_splitCount; i++)
    {
        m_freezeData.speed = rng.getRandomFloat(FREEZE_GRENADE_SPEED_LOW, FREEZE_GRENADE_SPEED_HIGH);
        dirMod = rng.getRandomFloat(FREEZE_GRENADE_SPLIT_DIR_LOW, FREEZE_GRENADE_SPLIT_DIR_HIGH);
        upMod = rng.getRandomFloat(FREEZE_GRENADE_SPLIT_UPDIR_LOW, FREEZE_GRENADE_SPLIT_UPDIR_HIGH);

        Projectile* p = getSpawnProjectileFunc()(m_freezeData, position, btVector3(cos(m_sliceSize * i) * dirMod, upMod, sin(m_sliceSize * i) * dirMod).normalize(), *data.caller, { 0.f, 0.f, 0.f });
        if (p)
        {
            p->addCallback(Entity::ON_DESTROY, [&](Entity::CallbackData &data) -> void {
                // explosion projectile
                m_explosionData.hitboxScale = FREEZE_GRENADE_SPLIT_EXPLOSION_SCALE;
                Projectile* explosion = getSpawnProjectileFunc()(m_explosionData, data.caller->getPositionBT(), { 0.f, 0.f, 0.f }, *data.caller, { 0.f, 0.f, 0.f });

                if (explosion)
                {
                    // Graphical explosion - particle
                    Graphics::FXSystem->addEffect("IceExplosion", data.caller->getPosition());

                    //explosion callback
                    explosion->addCallback(Entity::ON_COLLISION, [&](Entity::CallbackData &data) -> void {

                        data.caller->getSoundSource()->playSFX(Sound::SFX::WEAPON_ICEGUN_THIRD, 1.f, 0.33f);

                        if (Projectile* p = dynamic_cast<Projectile*>(data.caller))
                            p->getProjectileData().type = ProjectileTypeNormal;

                        PhysicsObject* obj = reinterpret_cast<PhysicsObject*>(data.dataPtr);

                        if (Enemy* enemy = dynamic_cast<Enemy*>(obj))
                        {
                            btVector3 knockbackDir = (obj->getPositionBT() - data.caller->getPositionBT()).normalize();
                            obj->getRigidBody()->setLinearVelocity(knockbackDir * 8.f);
                            enemy->getStatusManager().addStatusResetDuration(StatusManager::EFFECT_ID::STUN, 1);
                        }
                    });
                }
            });
        }
    }
}

bool WeaponFreezeGrenade::useEnhanced(bool shouldUse)
{
    if (shouldUse)
        m_explosionData.damage = FREEZE_GRENADE_DMG_ENHANCED;
    else
        m_explosionData.damage = FREEZE_GRENADE_DMG;

    m_enhanced = shouldUse;
    return m_enhanced;
}
