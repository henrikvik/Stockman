#ifndef GAMETIME_H
#define GAMETIME_H

#define GAME_TIME_NORMAL_TIME		1.f
#define GAME_TIME_EASING			12.5f
#define GAME_TIME_EASING_THRESHOLD	0.95f
#define GAME_TIME_EASING_OFFSET		0.05f
#define GAME_TIME_MS_TO_SEC			0.0001f

namespace Logic
{
	struct GameTime
	{
		GameTime() : targetMod(1.f), currentMod(1.f), dt(0), dtReal(0), duration(-1), done(false) { }

		void slowMotion(float modifier, float duration)
		{
			this->targetMod = modifier;
			this->duration = duration;
			this->done = false;
		}

		void update(float deltaTime)
		{
			dtReal = deltaTime;

			// If slowdown effect is completed
			if (duration < 0)	targetMod = GAME_TIME_NORMAL_TIME;
			else				duration -= deltaTime;

			// Easing the slowdown to the target
			if (!done)
			{
				currentMod += (targetMod - currentMod) * GAME_TIME_EASING * (dtReal * GAME_TIME_MS_TO_SEC);
				if (targetMod > GAME_TIME_EASING_THRESHOLD && targetMod - currentMod < GAME_TIME_EASING_OFFSET)
				{
					currentMod = targetMod;
					done = true;
				}
			}

			// Changing the deltatime with the current modifier
			dt = currentMod * dtReal;
		}

		bool done;			// If the easing function can be avoided
		float duration;		// The duration of the game time adjustment, in milliseconds
		float targetMod;	// Target Modifer (for the easing function)
		float currentMod;	// Current Modifier
		float dt;			// Deltatime with slow & speedup effects
		float dtReal;		// Normal deltatime without modifier (for player movement etc.)
	};
}

#endif // !GAMETIME_H
