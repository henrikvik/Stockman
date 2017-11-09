#include <Player\Weapon\WeaponFreezeGrenade.h>
#include <Projectile\Projectile.h>
#include <Projectile\ProjectileStruct.h>


using namespace Logic;

WeaponFreezeGrenade::WeaponFreezeGrenade()
{
}

WeaponFreezeGrenade::WeaponFreezeGrenade(ProjectileManager * projectileManager, ProjectileData & projectileData, WeaponInfo wInfo, ProjectileData & freezeData, int splitCount)
    : Weapon(projectileManager, projectileData, wInfo)
{
    m_freezeData = newd ProjectileData(freezeData);
    m_splitCount = splitCount;
    sliceSize = (2 * 3.14f) / m_splitCount;
}


WeaponFreezeGrenade::~WeaponFreezeGrenade()
{
    delete m_freezeData;
}

void WeaponFreezeGrenade::onUse(std::vector<Projectile*> &projectiles, Entity& shooter)
{
    for (Projectile* p : projectiles)
    {
        p->addCallback(Entity::ON_COLLISION, [&](Entity::CallbackData &data) -> void {
            
            PhysicsObject* obj = reinterpret_cast<PhysicsObject*>(data.dataPtr);

            btVector3 position;

            if (dynamic_cast<Entity*>(obj))
                position = data.caller->getPositionBT() + btVector3(0.f, 0.1f, 0.f);
            else
                position = data.caller->getPositionBT() + btVector3(0.f, 1.f, 0.f);

            for (int i = 0; i < m_splitCount; i++)
            {
                Projectile* p = getSpawnProjectileFunc()(*m_freezeData, position, btVector3(cos(sliceSize * i), 0.f, sin(sliceSize * i)), *data.caller);
                p->addCallback(Entity::ON_COLLISION, [&](Entity::CallbackData &data) -> void {
                    
                    PhysicsObject* obj = reinterpret_cast<PhysicsObject*>(data.dataPtr);

                    for (int i = 0; i < m_splitCount; i++)
                        Projectile* p = getSpawnProjectileFunc()(*m_freezeData, data.caller->getPositionBT(), btVector3(cos(sliceSize * i), 0.f, sin(sliceSize * i)), *data.caller);
                });
            }
        });
    }
}
