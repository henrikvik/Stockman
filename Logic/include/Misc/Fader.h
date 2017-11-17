#ifndef FADER_H
#define FADER_H

namespace Logic
{
    struct Fader
    {
        enum FadingStyle { Nothing, FadeIn, FadeOut };
        Fader() : complete(true), threshold(0), start(0), timer(0), style(Nothing) { }

        // Goes from zero to "timeInMS" 
        void startFadeIn(float timeInMs)
        {
            complete      = false;
            threshold     = timeInMs;
            start         = timeInMs;
            timer         = 0;
            style         = FadeIn;
        }

        // Goes from "timeInMs" down to zero 
        void startFadeOut(float timeInMs)
        {
            complete      = false;
            threshold     = 0;
            timer         = timeInMs;
            start         = timeInMs;
            style         = FadeOut;
        }

        // Update the fader's timer and checks if done or not
        void update(float deltaTime)
        {
            // Don't calculate anything if we're done
            if (complete)
                return;

            // Fade in calculations
            if (style == FadeIn)
            {
                timer += deltaTime;
                if (timer > threshold)
                {
                    timer = threshold;
                    complete = true;
                }
            }

            // Fad out calculations
            if (style == FadeOut)
            {
                timer -= deltaTime;
                if (timer < threshold) 
                {
                    timer = threshold;
                    complete = true; 
                }
            }
        }

        // Gives a value from (0:1) where each type calculates this differently
        float getCurrentPercentage()
        {
            // Calculate the percentage left
            if (style == FadeIn)  return timer / threshold; // Counts from 1 and down to 0
            if (style == FadeOut) return timer / start;     // Counts from 0 and up to 1

            // No fading style
            return 0.f;
        }

        FadingStyle     style;      //< Different types of fading styles
        bool            complete;   //< If this fader have completed it's current course
        float           timer;      //< Current counter in MS
        float           threshold;  //< The clock in MS
        float           start;      //< To calculate fadeout percentage
    };
}

#endif // !FADER_H
