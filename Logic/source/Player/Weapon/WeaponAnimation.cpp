#include <Player\Weapon\WeaponAnimation.h>

using namespace Logic;

/* ENHANCED OPTIONS */
#define ENHANCED_ANIMATION_SPEED    0.0015f         // The easing amount it takes for the enhanced effect to accur
#define NOT_ENHANCED_COLOR          1.0, 1.0, 1.0   // NOT enhanced color
#define DEFAULT_ENHANCED_COLOR      1.0, 0.90, 0.95 // ~Crossbow
#define SLEDGE_ENHANCED_COLOR       0.6, 0.8, 1.f   // ~Sledge Hammer
#define FREEZE_ENHANCED_COLOR       0.85, 1.0, 0.95 // ~Freeze Gun
#define ENHANCED_SCALE              1.125f          // The scaling of the weapon when enhanced, default 1.0f 

/* ANIMATION OFFSETS */
#define RELOAD_TILT                 0.75f           // The amount of tilt to the right when reloading
#define RELOAD_BACK                 0.75f           // The amount of push backwards into the player when reloading
#define SWAP_BACK                   0.50f           // The amount of push backwards into the player when swapping weapons
#define SWAP_TILT                   0.50f           // The amount of tilt to the right when swapping weapons
#define WINDUP_TILT                 1.00f           // The amount of tilt to the right when using windup
#define WINDUP_BACK                 0.75f           // The amount of push backwards into the player when using windup
#define WINDUP_ANGLE                45.0f           // The rotation in X in degrees to rotate when windup
#define SHOOT_BACK                  0.25f           // The amount of force back to the player when shooting

WeaponAnimation::WeaponAnimation(Resources::Models::Files modelID, AnimationFrame defaultFrame)
    : ModelAnimation(modelID, defaultFrame)
{
    m_enhancedColorDefault = DirectX::SimpleMath::Vector3(DEFAULT_ENHANCED_COLOR);
    m_enhancedColorCurrent = DirectX::SimpleMath::Vector3(NOT_ENHANCED_COLOR);
    m_enhancedColorTarget = DirectX::SimpleMath::Vector3(NOT_ENHANCED_COLOR);
    m_isEnhanced = false;
}

void WeaponAnimation::startReloadingAnimation(float reloadTime)
{
    std::queue<AnimationFrame> frames;

    // Just move the inwards to the player model, outside view
    AnimationFrame frame = m_frameDefault;
    frame.translation._42 -= RELOAD_TILT;
    frame.translation._43 += RELOAD_BACK;
    frames.push(frame);

    addAnimation(reloadTime, frames);
}

void WeaponAnimation::startSwapToAnimation(float swaptimer)
{
    // Forcing the current position a spot below the player
    m_frameActive = m_frameDefault;
    m_frameActive.translation._42 = m_frameDefault.translation._42 - SWAP_BACK;
    m_frameActive.translation._43 = m_frameDefault.translation._43 + SWAP_TILT;
}

void WeaponAnimation::startWindupAnimation(float delayTimer)
{
    std::queue<AnimationFrame> frames;

    AnimationFrame frame = m_frameDefault;

    // Moving backwards and to the right
    frame.translation._41 += WINDUP_TILT;
    frame.translation._43 += WINDUP_BACK;

    // Rotating 45 degrees 
    DirectX::SimpleMath::Matrix rotationMatrix = m_frameDefault.rotation;
    float angle = WINDUP_ANGLE * 3.14 / 180.f;
    rotationMatrix._11 += cos(angle);
    rotationMatrix._12 += sin(angle);
    rotationMatrix._21 += -sin(angle);
    rotationMatrix._22 += cos(angle);
    frame.rotation = rotationMatrix;

    frames.push(frame);

    addAnimation(delayTimer, frames);
}

void WeaponAnimation::startShootAnimation(float attackTimer, bool primary)
{
    std::queue<AnimationFrame> frames;

    // Just move the inwards to the player model, outside view
    AnimationFrame frame = m_frameDefault;
    frame.translation._43 += SHOOT_BACK;
    frames.push(frame);

    addAnimation(attackTimer, frames);
}

void WeaponAnimation::update(float dt, DirectX::SimpleMath::Matrix offsetTranslation, DirectX::SimpleMath::Vector3 offsetForward)
{
    // Enhanced Easing
    if (m_isEnhanced)
    {
        m_enhancedColorTarget = m_enhancedColorDefault;
        m_enhancedScaleTarget._11 = ENHANCED_SCALE;
        m_enhancedScaleTarget._22 = ENHANCED_SCALE;
        m_enhancedScaleTarget._33 = ENHANCED_SCALE;
    }
    else
    {
        m_enhancedColorTarget = DirectX::SimpleMath::Vector3(NOT_ENHANCED_COLOR);
        m_enhancedScaleTarget._11 = 1.f;
        m_enhancedScaleTarget._22 = 1.f;
        m_enhancedScaleTarget._33 = 1.f;
    }

    ModelAnimation::update(dt, offsetTranslation, offsetForward);

    // Enhanced Easing
    m_enhancedColorCurrent += (m_enhancedColorTarget - m_enhancedColorCurrent) * ENHANCED_ANIMATION_SPEED * dt;
    m_enhancedScaleCurrent += (m_enhancedScaleTarget - m_enhancedScaleCurrent) * ENHANCED_ANIMATION_SPEED * dt;
    m_model.color = m_enhancedColorCurrent;
    m_model.transform = m_enhancedScaleCurrent * m_model.transform;
}

/*************************
* Sledge Hammer Animation*
*************************/

WeaponSledgeHammerAnimation::WeaponSledgeHammerAnimation(Resources::Models::Files modelID, AnimationFrame defaultFrame)
    : WeaponAnimation(modelID, defaultFrame)
{
    m_enhancedColorDefault = DirectX::SimpleMath::Vector3(SLEDGE_ENHANCED_COLOR);
}

void WeaponSledgeHammerAnimation::startShootAnimation(float attackTimer, bool primary)
{
    std::queue<AnimationFrame> frames;
    float halfAttackTimer = attackTimer / 2.f;

    if (!primary)
    {
        // Parry
        AnimationFrame frame = m_frameDefault;
        frame.translation._41 -= 0.1f;
        frame.translation._42 += 0.5f;
        frame.translation._43 += 0.3f;
        frames.push(frame);
    }
    else
    {
        // Sledge Hammer
        AnimationFrame frame = m_frameDefault;
        frame.translation._41 += 0.6f;
        frame.translation._42 += 0.6f;
        frame.translation._43 += 1.25f;
        DirectX::SimpleMath::Matrix rotationMatrix;
        DirectX::SimpleMath::Matrix x = rotationMatrix.CreateRotationX((0.f * 3.14) / 180.f);
        DirectX::SimpleMath::Matrix y = rotationMatrix.CreateRotationY((105.f * 3.14) / 180.f);
        DirectX::SimpleMath::Matrix z = rotationMatrix.CreateRotationZ((287.f * 3.14) / 180.f);
        frame.rotation = z * y * x;
        frames.push(frame);
    }

    addAnimation(halfAttackTimer, frames);
}

/*************************
* Freeze Gun Animation   *
*************************/

WeaponFreezeGunAnimation::WeaponFreezeGunAnimation(Resources::Models::Files modelID, AnimationFrame defaultFrame)
    : WeaponAnimation(modelID, defaultFrame)
{
    m_enhancedColorDefault = DirectX::SimpleMath::Vector3(FREEZE_ENHANCED_COLOR);
}

void WeaponFreezeGunAnimation::startShootAnimation(float attackTimer, bool primary)
{
    if (!primary)
    {
        std::queue<AnimationFrame> frames;

        // Freeze Bomb
        AnimationFrame frame = m_frameDefault;
        frame.translation._42 -= 0.20f;
        frame.translation._43 -= 0.25f;
        frames.push(frame);

        addAnimation(attackTimer / 2.f, frames);
    }
    else
    {
        // Freeze Spray
        std::queue<AnimationFrame> frames;

        AnimationFrame frame = m_frameDefault;
        frame.translation._42 += 0.13;
        frame.translation._43 += 0.09;
        DirectX::SimpleMath::Matrix rotationMatrix;
        DirectX::SimpleMath::Matrix x = rotationMatrix.CreateRotationX((344.8f * 3.14) / 180.f);
        DirectX::SimpleMath::Matrix y = rotationMatrix.CreateRotationY((27.f * 3.14) / 180.f);
        DirectX::SimpleMath::Matrix z = rotationMatrix.CreateRotationZ((0.0 * 3.14) / 180.f);
        frame.rotation = z * y * x;
        frames.push(frame);
        m_frameActive = frame;

        addAnimation(attackTimer, frames);
    }
}