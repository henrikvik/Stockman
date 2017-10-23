#ifndef NOISESTRUCTS_H
#define NOISESTRUCTS_H

#include <fmod.hpp>
#include <btBulletCollisionCommon.h>

struct THRESHOLD
{
	static const int	MAX_CHANNELS	= 100;
	static const int	MAX_GROUPS		= 4;
	static const int	MAX_SFX			= 32;
	static const int	MAX_SONGS		= 32;
	static const int	MUSIC_MIN_DIST	= 1;
	static const int	MUSIC_MAX_DIST	= 25;
	static const int	SFX_MIN_DIST	= 1;
	static const int	SFX_MAX_DIST	= 10;
};

enum CHANNEL_GROUP 
{
	CHANNEL_MASTER,
	CHANNEL_SFX,
	CHANNEL_MUSIC,
	CHANNEL_AMBIENT
};

// Put all identifier for sfx here
enum SFX 
{
	BOING
};

// Put all identifier for music here
enum MUSIC 
{
	TEST_MUSIC,
	ENRAGE
};

// Keeps track of everything about a specific sound
struct Sound
{
	Sound() : channel(nullptr), data(nullptr), group(CHANNEL_GROUP(0)) { }

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
		vel =		{ inVelocity.x(),	inVelocity.y(), inVelocity.z() };
		up =		{ inUp.x(),			inUp.y(),		inUp.z() };
		forward =	{ inForward.x(),	inForward.y(),	inForward.z() };
		pos =		{ inPosition.x(),	inPosition.y(), inPosition.z() };
	}

	__int8		id;			//< Different id for different players
	FMOD_VECTOR vel;
	FMOD_VECTOR up;
	FMOD_VECTOR forward;
	FMOD_VECTOR pos;
};


#endif // !NOISESTRUCTS_H
