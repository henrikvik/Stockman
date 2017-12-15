#include <AI\Trigger\TriggerAmmoPickup.h>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace Logic;

#define TRIGGER_AMMO_PICKUP_BOBBING_OFFSET { 0.f, 0.3f, 0.f }
#define TRIGGER_AMMO_PICKUP_ROTATION_SPEED 0.002f;

TriggerAmmoPickup::TriggerAmmoPickup(Resources::Models::Files modelID, btRigidBody* body, btVector3 halfExtent, TriggerType type, float cooldown, bool reusable)
    : Trigger(modelID, body, halfExtent, type, cooldown, reusable)
{
    m_pos = getPositionBT();
    m_rotation = 0;

    /*addCallback(Entity::ON_COLLISION, [&](Entity::CallbackData &data) -> void {
        data.caller->getRigidBody()->setGravity({ 0.f, 0.f, 0.f });
        m_pos = getPositionBT();
    });*/
}


TriggerAmmoPickup::~TriggerAmmoPickup()
{
}

void TriggerAmmoPickup::updateSpecificType(float deltaTime)
{
    m_rotation += deltaTime * TRIGGER_AMMO_PICKUP_ROTATION_SPEED;
    
    getRigidBody()->getWorldTransform().setRotation(btQuaternion(m_rotation, 0.f, 0.f));

    //if(getRigidBody()->getGravity().y() > FLT_EPSILON)
        getRigidBody()->getWorldTransform().setOrigin(m_pos + sin(m_rotation) * btVector3(TRIGGER_AMMO_PICKUP_BOBBING_OFFSET));
}
