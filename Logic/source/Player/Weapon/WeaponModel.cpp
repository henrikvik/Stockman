#include <Player\Weapon\WeaponModel.h>
//#include <imgui.h>

using namespace Logic;

#define ANIMATION_EASING 0.010f
#define HEIGHT_POSITION_OFFSET_EASING   0.040f

WeaponModel::WeaponModel() { }

WeaponModel::WeaponModel(Resources::Models::Files modelID, WeaponModelAnimationInfo mInfo)
{
    m_mInfo = mInfo;
    renderInfo.model = modelID;
    m_animationCurrent = m_mInfo.trans;
    m_animationTarget = m_mInfo.trans;
    m_animationTimer = 0.f;
}

WeaponModel::~WeaponModel() { }

void WeaponModel::startShootAnimation()
{
    m_animationTarget._43 += 0.25f;
    m_animationTimer = 75.f;
}

void WeaponModel::startReloadAnimation()
{
    DirectX::SimpleMath::Matrix rotationMatrix;

    m_animationTarget._42 -= 0.75f;
    m_animationTarget._43 += 0.75f;
    m_animationTimer = 1000.f;
}

void WeaponModel::animation(float deltaTime)
{
    if (m_animationTimer < 0.f)
    {
        m_animationTarget = m_mInfo.trans;
    }
    else
    {
        m_animationTimer -= deltaTime;
    }

    // Easing to the target animation placement
    float easingVariable = min((ANIMATION_EASING / sqrt(m_mInfo.scale._22) * deltaTime), 1.f);
    m_animationCurrent += (m_animationTarget - m_animationCurrent) * easingVariable;
}

void WeaponModel::update(float deltaTime, DirectX::SimpleMath::Matrix playerTranslation, DirectX::SimpleMath::Vector3 playerForward)
{
    // Making a camera matrix and then inverting it 
    DirectX::SimpleMath::Matrix camera = DirectX::XMMatrixLookToRH({ 0, 0, 0 }, playerForward, { 0, 1, 0 });

    // Pushing the model forward in the current view direction
    DirectX::SimpleMath::Matrix offset = (DirectX::SimpleMath::Matrix::CreateTranslation(playerTranslation.Translation() + playerForward));
    DirectX::SimpleMath::Matrix result = m_mInfo.rot * m_animationCurrent * m_mInfo.scale * camera.Invert() * offset;
    DirectX::SimpleMath::Matrix temp = m_current;
    m_current += (result - m_current);

    // Easing in Y axis
    m_current._42 = temp._42;
    float easingVariable = min((HEIGHT_POSITION_OFFSET_EASING / sqrt(m_mInfo.scale._22) * deltaTime), 1.f);
    m_current._42 += (result._42 - m_current._42) * easingVariable;

    renderInfo.transform = m_current;

    animation(deltaTime);
}

const WeaponModel::WeaponModelAnimationInfo& Logic::WeaponModel::getModelInfo() const
{
    return m_mInfo;
}

void WeaponModel::render() const
{
    QueueRender(renderInfo);
}
