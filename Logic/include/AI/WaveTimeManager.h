#ifndef WAVE_TIME_MANAGER_H
#define WAVE_TIME_MANAGER_H

namespace Logic
{
    class EntityManager;

    class WaveTimeManager
    {
    private:
        static const float TRANSITION_TIME;
        float m_timeCurrent;
        float m_timeRequired;

        int m_waveCurrent;
        bool m_onLastWave, m_enraged, m_onTransition;
    public:
        WaveTimeManager();
        virtual ~WaveTimeManager();

        bool update(float deltaTime, EntityManager &entityManager);
        void reset();

        int getCurrentWave() const;
        float getTimeCurrent() const;
        float getTimeRequired() const;

        bool onLastWave() const;
    };
};

#endif