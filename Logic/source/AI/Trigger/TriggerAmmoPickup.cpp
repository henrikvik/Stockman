#include <AI\Trigger\TriggerAmmoPickup.h>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace Logic;


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
    //getRigidBody()->getWorldTransform().setRotation(btQuaternion(sin(deltaTime) * 2.f * M_PI * 0.1f, 0.f, 0.f));
}
