#ifndef WAVE_TIME_MANAGER_H
#define WAVE_TIME_MANAGER_H

class btVector3;
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

        bool update(float deltaTime, EntityManager &entityManager, btVector3 const &playerPos);

        void reset();
        void startTransition();

        int getCurrentWave() const;
        float getTimeCurrent() const;
        float getTimeRequired() const;
        bool getOnLastWave() const;

        bool onLastWave() const;
        bool isEnraged() const;
        bool isTransitioning() const;
    };
};

#endif