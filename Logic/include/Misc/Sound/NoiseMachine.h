#ifndef NOISEMACHINE_H
#define NOISEMACHINE_H

#include <Engine\Constants.h>	// Paths to folders
#include <fmod.hpp>
#include <fmod_errors.h>
#include <iostream>
#include <stdarg.h>

/* 

	// if it eats up memory
	ERRCHECK(FMOD::Memory_Initialize(malloc(4*1024*1024), 4*1024*1024, 0,0,0));

*/

namespace Logic
{
	static int MAX_CHANNELS = 32;

	class NoiseMachine
	{
	public:
		static NoiseMachine& Get()
		{
			static NoiseMachine noiceMachine;
			return noiceMachine;
		}

		void init();

		void update();

		void playSound();


	private:

		void CRASH_EVERYTHING(const char *format, ...);
		void ERRCHECK(FMOD_RESULT result);

		FMOD::System*	m_system;
		FMOD::Channel*	m_channel;
		FMOD::Sound*	m_sound;
	};
}

#endif // !NOICEMACHINE_H
