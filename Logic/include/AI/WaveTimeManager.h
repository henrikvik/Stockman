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

        void update(float deltaTime, EntityManager &entityManager);

        int getCurrentWave() const;
        float getTimeCurrent() const;
        float getTimeRequired() const;

        bool onLastWave() const;
    };
};

#endif