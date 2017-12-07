#ifndef WEAPONANIMATION_H
#define WEAPONANIMATION_H

#include <Player\Weapon\ModelAnimation.h>

namespace Logic
{
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

#endif // !WEAPONANIMATION_H
