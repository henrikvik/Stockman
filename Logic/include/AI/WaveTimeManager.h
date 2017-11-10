#ifndef WAVE_TIME_MANAGER_H
#define WAVE_TIME_MANAGER_H

namespace Logic
{
    class EntityManager;

    class WaveTimeManager
    {
    private:
        float m_timeCurrent;
        float m_timeRequired;

        int m_waveCurrent;
        bool m_onLastWave, m_enraged;
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