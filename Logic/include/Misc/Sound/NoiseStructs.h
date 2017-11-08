#ifndef NOISESTRUCTS_H
#define NOISESTRUCTS_H

#include <fmod.hpp>
#include <btBulletCollisionCommon.h>

namespace Sound
{
    enum LOAD_MODE { SAMPLE, STREAM };

    // Put all identifier for sfx here
    enum SFX
    {
        SWOOSH,
        FOOTSTEP_SMALL,
        FOOTSTEP_SNOW,
        JUMP,
        WEAPON_CUTLERY_PRIMARY,
        WEAPON_CUTLERY_SECONDARY,
        WEAPON_ICEGUN_PRIMARY,
        WEAPON_ICEGUN_SECONDARY,
        WEAPON_MELEE_PRIMARY,
        WEAPON_MELEE_SECONDARY,
        NECROMANCER_DEATH,
        NECROMANCER_SPAWN,
        NECROMANCER_SHOOT_PRIMARY,
        NECROMANCER_SHOOT_SECONDARY
    };

    // Put all identifier for music here
    enum MUSIC
    {
        MUSIC_MAIN_MENU,
        MUSIC_CREDITS,
        MUSIC_IN_GAME,
        AMBIENT_STORM
    };

    enum CHANNEL_GROUP
    {
        CHANNEL_MASTER,
        CHANNEL_SFX,
        CHANNEL_MUSIC,
        CHANNEL_AMBIENT
    };

    // Keeps track of everything about a specific sound
    struct Noise
    {
        Noise() : channel(nullptr), data(nullptr), group(CHANNEL_GROUP(0)) { }

        FMOD::Channel*	channel;
        FMOD::Sound*	data;
        CHANNEL_GROUP	group;
    };

    // The "Ears"
    struct ListenerData
    {
        ListenerData() : id(0), vel({ 0.f, 1.f, 0.f }), up({ 0.f, 1.f, 0.f }), forward({ 0.f, 0.f, 1.f }), pos({ 0.f, 0.f, 0.f }) { }

        void update(btVector3 inVelocity, btVector3 inUp, btVector3 inForward, btVector3 inPosition)
        {
            vel     = { inVelocity.x(),	inVelocity.y(), inVelocity.z()  };
            up      = { inUp.x(),			inUp.y(),		inUp.z()    };
            forward = { inForward.x(),	inForward.y(),	inForward.z()   };
            pos     = { inPosition.x(),	inPosition.y(), inPosition.z()  };
        }

        __int8		id;			//< Different id for different players
        FMOD_VECTOR vel;
        FMOD_VECTOR up;
        FMOD_VECTOR forward;
        FMOD_VECTOR pos;
    };
};



#endif // !NOISESTRUCTS_H
