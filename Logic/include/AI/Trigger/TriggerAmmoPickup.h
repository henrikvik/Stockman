#ifndef TRIGGERAMMOPICKUP_H
#define TRIGGERAMMOPICKUP_H

#include <AI\Trigger\Trigger.h>

namespace Logic
{
    class TriggerAmmoPickup : public Trigger
    {
    public:
        TriggerAmmoPickup(Resources::Models::Files modelID, btRigidBody* body, btVector3 halfExtent, TriggerType type, float cooldown, bool reusable);
        virtual ~TriggerAmmoPickup();

        virtual void updateSpecificType(float deltaTime);

    private:
        btVector3 m_pos;
        float m_rotation;
    };
}

#endif // !TRIGGERAMMOPICKUP_H

