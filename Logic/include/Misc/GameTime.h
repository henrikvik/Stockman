#ifndef GAMETIME_H
#define GAMETIME_H

#define GAME_TIME_EASING	12.5f

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
			if (duration < 0)	targetMod = 1.f;
			else				duration -= deltaTime;

			// Easing the slowdown to the target
			if (!done)
			{
				currentMod += (targetMod - currentMod) * GAME_TIME_EASING * (dtReal * 0.0001);
				if (targetMod > 0.95f && targetMod - currentMod < 0.05f )
				{
					currentMod = targetMod;
					done = true;
				}
			}

			// Changing the deltatime with the current modifier
			dt = currentMod * dtReal;
		}

		bool done;
		float duration;		// The duration of the game time adjustment, in milliseconds
		float targetMod;	// Target Modifer (for the easing function)
		float currentMod;	// Current Modifier
		float dt;			// Deltatime with slow & speedup effects
		float dtReal;		// Normal deltatime without modifier (for player movement etc.)
	};
}

#endif // !GAMETIME_H
