#ifndef BIG_BAD_BEHAVIOR_H
#define BIG_BAD_BEHAVIOR_H

#include "Behavior.h"

namespace Logic
{
    class BigBadBehavior : public Behavior
    {
        private:
            static const float MELEE_RANGE;
            static const int PHASE_ONE, PHASE_TWO, PHASE_THREE;
        public:
            BigBadBehavior();
            virtual ~BigBadBehavior();
    };
}

#endif