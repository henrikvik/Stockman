#ifndef STAY_BEHAVIOR_H
#define STAY_BEHAVIOR_H

#include "Behavior.h"
namespace Logic
{
    /*
        This is a simple behavior if you want no movement and only abilities
    */
    class StayBehavior : public Behavior
    {
        public:
            StayBehavior();
            virtual ~StayBehavior();
    };
}

#endif