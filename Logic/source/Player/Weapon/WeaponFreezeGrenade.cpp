#include <Player\Weapon\WeaponFreezeGrenade.h>
#include <Projectile\Projectile.h>
#include <Projectile\ProjectileStruct.h>
#include <Misc\Sound\NoiseMachine.h>

#define _USE_MATH_DEFINES
#include <math.h>


using namespace Logic;

#define FREEZE_GRENADE_SPEED_LOW    4.f
#define FREEZE_GRENADE_SPEED_HIGH   16.f
#define FREEZE_GRENADE_SPLIT_DIR_LOW 0.8f
#define FREEZE_GRENADE_SPLIT_DIR_HIGH 1.2f
#define FREEZE_GRENADE_SPLIT_UPDIR_LOW 2.f
#define FREEZE_GRENADE_SPLIT_UPDIR_HIGH 4.f

WeaponFreezeGrenade::WeaponFreezeGrenade()
{
}

WeaponFreezeGrenade::WeaponFreezeGrenade(ProjectileManager * projectileManager, ProjectileData & projectileData, WeaponInfo wInfo, ProjectileData & freezeData, int splitCount)
    : Weapon(projectileManager, projectileData, wInfo)
{
    m_freezeData = freezeData;
    m_splitCount = splitCount;
    m_sliceSize = (2 * M_PI) / m_splitCount;
}


WeaponFreezeGrenade::~WeaponFreezeGrenade()
{

}

void WeaponFreezeGrenade::onUse(std::vector<Projectile*> &projectiles, Entity& shooter)
{
    Sound::NoiseMachine::Get().playSFX(Sound::SFX::WEAPON_ICEGUN_SECONDARY, nullptr, true);

    for (Projectile* p : projectiles)
    {
        p->addCallback(Entity::ON_COLLISION, [&](Entity::CallbackData &data) -> void {
            
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
                    p->addCallback(Entity::ON_COLLISION, [&](Entity::CallbackData &data) -> void {

                        PhysicsObject* obj = reinterpret_cast<PhysicsObject*>(data.dataPtr);

                        float dirMod;
                        float upMod;

                        for (int i = 0; i < m_splitCount; i++)
                        {
                            m_freezeData.speed = rng.getRandomFloat(FREEZE_GRENADE_SPEED_LOW, FREEZE_GRENADE_SPEED_HIGH);
                            dirMod = rng.getRandomFloat(FREEZE_GRENADE_SPLIT_DIR_LOW, FREEZE_GRENADE_SPLIT_DIR_HIGH);
                            upMod = rng.getRandomFloat(FREEZE_GRENADE_SPLIT_UPDIR_LOW, FREEZE_GRENADE_SPLIT_UPDIR_HIGH);
                            Projectile* p = getSpawnProjectileFunc()(m_freezeData, data.caller->getPositionBT(), btVector3(cos(m_sliceSize * i), upMod, sin(m_sliceSize * i)).normalize(), *data.caller, { 0.f, 0.f, 0.f });
                        }

                    });
                }
            }

        });
    }
}

bool Logic::WeaponFreezeGrenade::useEnhanced(bool shouldUse)
{
    return shouldUse;
}
