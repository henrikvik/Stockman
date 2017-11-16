#include <AI\Trigger\TriggerAmmoPickup.h>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace Logic;

#define TRIGGER_AMMO_PICKUP_BOBBING_OFFSET { 0.f, 0.3f, 0.f }

TriggerAmmoPickup::TriggerAmmoPickup(Resources::Models::Files modelID, btRigidBody* body, btVector3 halfExtent, TriggerType type, float cooldown, bool reusable)
    : Trigger(modelID, body, halfExtent, type, cooldown, reusable)
{
    m_pos = getPositionBT();
}


TriggerAmmoPickup::~TriggerAmmoPickup()
{
}

void Logic::TriggerAmmoPickup::updateSpecificType(float deltaTime)
{
    Entity::update(deltaTime);
    static float yaw = 0;
    yaw += deltaTime * 0.001f;
    
    getRigidBody()->getWorldTransform().setRotation(btQuaternion(yaw, 0.f, 0.f));
    getRigidBody()->getWorldTransform().setOrigin(m_pos + sin(yaw) * btVector3(TRIGGER_AMMO_PICKUP_BOBBING_OFFSET));
}
