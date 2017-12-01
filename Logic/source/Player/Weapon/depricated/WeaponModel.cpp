#include <Player\Weapon\depricated\WeaponModel.h>
#include <Keyboard.h>
#include <imgui.h>

using namespace Logic;

#define ANIMATION_EASING 0.020f
#define HEIGHT_POSITION_OFFSET_EASING 0.038f

WeaponModel::WeaponModel() { }

WeaponModel::WeaponModel(Resources::Models::Files modelID, WeaponModelAnimationInfo mInfo, AnimationType animationType)
{
    // Model information
    m_mInfo                 = mInfo;
    renderInfo.model        = modelID;

    // Animation
    m_animationCurrent      = m_mInfo.trans;
    m_animationTarget       = m_mInfo.trans;
    m_animationCurrentRot   = m_mInfo.rot;
    m_animationTargetRot    = m_mInfo.rot;
    m_animationTimer        = 0.f;
    m_animationType         = animationType;
}

WeaponModel::~WeaponModel() { }

// Starts the windup animation for this weapon model
// \param the amount of length that should be translated back to the player
void WeaponModel::startSwapToAnimation(float holsterAmount)
{
    m_animationTarget = m_mInfo.trans;
    m_animationCurrent._42 = m_animationTarget._42 - holsterAmount;
    m_animationCurrent._43 = m_animationTarget._43 + holsterAmount;
    m_animationTimer = m_mInfo.swapTimer;
}

// Starts the windup animation for this weapon model
// \param backpower The amount of length upwards the weapon should be 
// \param delayTimer The amount of time it takes for the weapon to delay until attack
void WeaponModel::startWindupAnimation(float backPower, float delayTimer)
{
    m_animationTarget = m_mInfo.trans;
    m_animationTimer = delayTimer;

    if (m_animationType == Hammer)
    {
        m_animationTarget._41 += backPower;
        m_animationTarget._43 += backPower * 0.75;

        DirectX::SimpleMath::Matrix rotationMatrix = m_mInfo.rot;
        float angle = 45.f * 3.14 / 180.f;
        rotationMatrix._11 += cos(angle);
        rotationMatrix._12 += sin(angle);
        rotationMatrix._21 += -sin(angle);
        rotationMatrix._22 += cos(angle);
        m_animationTargetRot = rotationMatrix;
    }
}

// Starts the shooting animation for this weapon model
// \param backpower The amount of power the model should knock back, default = 0.25
// \param attackTimer The fire rate of this weapon in milliseconds to avoid overlapping animations, default = 75.f
void WeaponModel::startShootAnimation(float backPower, float attackTimer, bool primary)
{
    m_animationTarget = m_mInfo.trans;
    m_animationTargetRot = m_mInfo.rot;
    m_animationTimer = attackTimer * 0.5f;

    if (m_animationType == Hammer)
    {
        if (!primary) 
        {
            m_animationTarget._41 -= 0.1f;
            m_animationTarget._42 += 0.5f;
            m_animationTarget._43 += 0.3f;
        }
        else
        {
            m_animationTarget._43 += backPower;
            m_animationTarget._41 += 0.6f;
            m_animationTarget._43 += 1.f;
            m_animationTarget._42 += 0.6f;

            DirectX::SimpleMath::Matrix rotationMatrix;
            DirectX::SimpleMath::Matrix x = rotationMatrix.CreateRotationX((0.f * 3.14) / 180.f);
            DirectX::SimpleMath::Matrix y = rotationMatrix.CreateRotationY((105.f * 3.14) / 180.f);
            DirectX::SimpleMath::Matrix z = rotationMatrix.CreateRotationZ((287.f * 3.14) / 180.f);

            m_animationTargetRot = z * y * x;
        }
    }
    else if (m_animationType == Ice)
    {
        if (primary)
        {
            m_animationTarget._43 -= backPower * 0.1;
        }
        else
        {
            m_animationTarget._43 += backPower * 1.5;
        }
    }
    else
    {
        m_animationTarget._43 += backPower;
    }
}

// Starts the reloading animation for this weapon model
// \param holsterAmount The amount of power the model should knock back, default = 0.75
// \param reloadTimer The reload rate of this weapon in milliseconds to avoid overlapping animations, default = 1000.f
void WeaponModel::startReloadAnimation(float holsterAmount, float reloadTimer)
{
    m_animationTarget = m_mInfo.trans;
    m_animationTarget._42 -= holsterAmount;
    m_animationTarget._43 += holsterAmount;
    m_animationTimer = reloadTimer;
}

void WeaponModel::animation(float deltaTime)
{
    // Checks to see if animation is over
    if (m_animationTimer < 0.f) 
    {
        m_animationTarget = m_mInfo.trans;
        m_animationTargetRot = m_mInfo.rot;
    }
    else m_animationTimer -= deltaTime;

    // Easing to the targeted animation placement
    float easingVariable = min((ANIMATION_EASING / sqrt(m_mInfo.scale._22) * deltaTime), 1.f);
    m_animationCurrent += (m_animationTarget - m_animationCurrent) * easingVariable;
    m_animationCurrentRot += (m_animationTargetRot - m_animationCurrentRot) * easingVariable;
}

void WeaponModel::update(float deltaTime, DirectX::SimpleMath::Matrix playerTranslation, DirectX::SimpleMath::Vector3 playerForward)
{
    // Making a camera matrix and then inverting it 
    DirectX::SimpleMath::Matrix camera = DirectX::XMMatrixLookToRH({ 0, 0, 0 }, playerForward, { 0, 1, 0 });

    // Pushing the model forward in the current view direction
    DirectX::SimpleMath::Matrix offset = (DirectX::SimpleMath::Matrix::CreateTranslation(playerTranslation.Translation() + playerForward));
    DirectX::SimpleMath::Matrix result = m_animationCurrentRot * m_animationCurrent * m_mInfo.scale * camera.Invert() * offset;
    DirectX::SimpleMath::Matrix temp = m_current;
    m_current += (result - m_current);

    // Easing in Y axis (Offset when jumping / flying)
    m_current._42 = temp._42;
    float easingVariable = min((HEIGHT_POSITION_OFFSET_EASING / sqrt(m_mInfo.scale._22) * deltaTime), 1.f);
    m_current._42 += (result._42 - m_current._42) * easingVariable;

    // Setting the final transform of this model
    renderInfo.transform = m_current;

    // Ease to the targeted animation
    animation(deltaTime);
}

WeaponModel::WeaponModelAnimationInfo& Logic::WeaponModel::getModelInfo()
{
    return m_mInfo;
}

void WeaponModel::render() const
{
    QueueRender(renderInfo);
}
