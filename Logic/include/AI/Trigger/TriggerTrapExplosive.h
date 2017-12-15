#include "Trigger.h"

namespace Logic
{
    class TriggerTrapExplosive : public Trigger 
    {
        private:
        public:
            TriggerTrapExplosive(btRigidBody* body);

            void onCollision(PhysicsObject& other, btVector3 contactPoint, float dmgMultiplier);
    };
}