#include <Player\Weapon\WeaponModel.h>
//#include <imgui.h>

using namespace Logic;

#define VERTICAL_POSITION_OFFSET_EASING 0.045f
#define HEIGHT_POSITION_OFFSET_EASING   0.040f

WeaponModel::WeaponModel() { }

WeaponModel::WeaponModel(Resources::Models::Files modelID, WeaponModelAnimationInfo mInfo)
{
    m_mInfo = mInfo;
    renderInfo.model = modelID;
}

WeaponModel::~WeaponModel() { }

void WeaponModel::update(float deltaTime, DirectX::SimpleMath::Matrix playerTranslation, DirectX::SimpleMath::Vector3 playerForward)
{
    /*static float ltrans[3];
    static float lscale[3] = {1, 1, 1};
    static float lrot[3];

    ImGui::Begin("asdoasdoasod");

    ImGui::DragFloat3("translate", ltrans, 0.1f);
    ImGui::DragFloat3("scale", lscale, 0.1f);
    ImGui::DragFloat3("rotation", lrot, 0.1f);

    ImGui::End();

    */

    // Making a camera matrix and then inverting it 
    DirectX::SimpleMath::Matrix camera = DirectX::XMMatrixLookToRH({ 0, 0, 0 }, playerForward, { 0, 1, 0 });

    // Pushing the model forward in the current view direction
    DirectX::SimpleMath::Matrix offset = (DirectX::SimpleMath::Matrix::CreateTranslation(playerTranslation.Translation() + playerForward));

    // Multiplying all the matrices into one
    DirectX::SimpleMath::Matrix result = m_mInfo.rot * m_mInfo.trans * m_mInfo.scale * camera.Invert() * offset;

    DirectX::SimpleMath::Matrix temp = m_current;
    m_current += (result - m_current);

    //// Easing in X axis
    //m_current._41 = temp._41;
    //m_current._41 += (result._41 - m_current._41) * 0.80;

    //// Easing in Z axis
    //m_current._43 = temp._43;
    //m_current._43 += (result._43 - m_current._43) * 0.80;

    // Easing in Y axis
    m_current._42 = temp._42;
    m_current._42 += (result._42 - m_current._42) * HEIGHT_POSITION_OFFSET_EASING * deltaTime;

    renderInfo.transform = m_current;
}

const WeaponModel::WeaponModelAnimationInfo& Logic::WeaponModel::getModelInfo() const
{
    return m_mInfo;
}

void WeaponModel::render() const
{
    QueueRender(renderInfo);
}
