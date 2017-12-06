#ifndef MODELANIMATION_H
#define MODELANIMATION_H

#include <Misc\NonCopyable.h>
#include <Graphics/include/RenderQueue.h>
#include <queue>

namespace Logic
{
    class ModelAnimation : public NonCopyable
    {
    public:

        struct AnimationFrame
        {
            DirectX::SimpleMath::Matrix rotation;
            DirectX::SimpleMath::Matrix translation;
            DirectX::SimpleMath::Matrix scale;
            float easingModifier;

            void easeTo(float dt, const AnimationFrame& other);
        };

        struct Animation
        {
            Animation();
            bool initialize(float totalTime, std::queue<AnimationFrame> inFrames);
            virtual void update(float dt);
            AnimationFrame getCurrentFrame();

            bool                        done;               //< If this animation is completed
            float                       timer;              //< The current time passed on current frame
            float                       timeBetweenFrames;  //< The time required to switch frames
            std::queue<AnimationFrame>  frames;             //< The full list of all animation frames
        };

        ModelAnimation(Resources::Models::Files modelID, AnimationFrame defaultFrame);
        ~ModelAnimation();

        void addAnimation(float totalDuration, std::queue<AnimationFrame> frames);
        virtual void update(float dt, DirectX::SimpleMath::Matrix offsetTranslation, DirectX::SimpleMath::Vector3 offsetForward);
        void render() const;

    protected:
        AnimationFrame      m_frameDefault;
        AnimationFrame      m_frameActive;
        AnimationFrame      m_frameTarget;
        StaticRenderInfo    m_model;

    private:
        Animation           m_animation;
    };

    class WeaponAnimation : public ModelAnimation
    {
    public:
        WeaponAnimation(Resources::Models::Files modelID, AnimationFrame defaultFrame);
        virtual void startReloadingAnimation(float reloadTime);
        virtual void startSwapToAnimation(float swaptimer);
        virtual void startWindupAnimation(float delayTimer);
        virtual void startShootAnimation(float attackTimer, bool primary);
        void update(float dt, DirectX::SimpleMath::Matrix offsetTranslation, DirectX::SimpleMath::Vector3 offsetForward);
        void setIsEnhanced(bool enhanced) { m_isEnhanced = enhanced; }

    protected:
        DirectX::SimpleMath::Matrix     m_enhancedScaleCurrent;
        DirectX::SimpleMath::Matrix     m_enhancedScaleTarget;
        DirectX::SimpleMath::Vector3    m_enhancedColorDefault;
        DirectX::SimpleMath::Vector3    m_enhancedColorCurrent;
        DirectX::SimpleMath::Vector3    m_enhancedColorTarget;
        bool                            m_isEnhanced;
    };

    class WeaponSledgeHammerAnimation : public WeaponAnimation
    {
    public:
        WeaponSledgeHammerAnimation(Resources::Models::Files modelID, AnimationFrame defaultFrame);
        void startShootAnimation(float attackTimer, bool primary);
    };

    class WeaponFreezeGunAnimation : public WeaponAnimation
    {
    public:
        WeaponFreezeGunAnimation(Resources::Models::Files modelID, AnimationFrame defaultFrame);
        void startShootAnimation(float attackTimer, bool primary);
    };
}

#endif // !MODELANIMATION_H
