#ifndef WEAPON_MODEL_H
#define WEAPON_MODEL_H

#include <Entity\Object.h>
#include <Graphics/include/RenderQueue.h>

namespace Logic
{
    class WeaponModel
    {
    public:

        enum AnimationType
        {
            Normal, Ice, Hammer
        };

        struct WeaponModelAnimationInfo
        {
            DirectX::SimpleMath::Matrix rot, trans, scale;
            float swapTimer;
        };

        WeaponModel();
        WeaponModel(Resources::Models::Files modelID, WeaponModelAnimationInfo mInfo, AnimationType = Normal);
        ~WeaponModel();

        // Fake animations
        virtual void startSwapToAnimation(float holsterAmount = 0.75f);
        virtual void startWindupAnimation(float backPower = 0.25f, float delayTimer = 250.f);
        virtual void startShootAnimation(float backPower = 0.25f, float attackTimer = 75.f, bool primary = true);
        virtual void startReloadAnimation(float holsterAmount = 0.75f, float reloadTimer = 1000.f);
        virtual void animation(float deltaTime);

        void update(float deltaTime, DirectX::SimpleMath::Matrix playerTranslation, DirectX::SimpleMath::Vector3 playerForward);

        const WeaponModelAnimationInfo& getModelInfo() const;

        void render() const;

    private:
        // Fake animation       
        float                       m_animationTimer;
        AnimationType               m_animationType;
        DirectX::SimpleMath::Matrix m_animationTarget;
        DirectX::SimpleMath::Matrix m_animationCurrent;
        DirectX::SimpleMath::Matrix m_animationTargetRot;
        DirectX::SimpleMath::Matrix m_animationCurrentRot;

        DirectX::SimpleMath::Matrix m_current;
        WeaponModelAnimationInfo m_mInfo;
        //PlayerRenderInfo renderInfo;
        StaticRenderInfo renderInfo; // temp
    };
}

#endif // !WEAPON_MODEL_H

