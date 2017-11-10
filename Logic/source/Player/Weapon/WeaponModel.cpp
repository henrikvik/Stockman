#include <Player\Weapon\WeaponModel.h>
//#include <imgui.h>

using namespace Logic;

WeaponModel::WeaponModel()
{
}

WeaponModel::WeaponModel(Resources::Models::Files modelID, WeaponModelAnimationInfo mInfo)
{
    m_mInfo = mInfo;
    renderInfo.model = modelID;
}


WeaponModel::~WeaponModel()
{
}

void Logic::WeaponModel::update(DirectX::SimpleMath::Matrix playerTranslation, DirectX::SimpleMath::Vector3 playerForward)
{
    /*static float ltrans[3];
    static float lscale[3] = {1, 1, 1};
    static float lrot[3];

    ImGui::Begin("asdoasdoasod");

    ImGui::DragFloat3("translate", ltrans, 0.1f);
    ImGui::DragFloat3("scale", lscale, 0.1f);
    ImGui::DragFloat3("rotation", lrot, 0.1f);

    ImGui::End();

    m_mInfo.trans = DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(ltrans));

    //// Scaling the model by making it thinner and longer
    m_mInfo.scale = DirectX::SimpleMath::Matrix::CreateScale(DirectX::SimpleMath::Vector3(lscale));
    m_mInfo.rot = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(lrot[0], lrot[1], lrot[2]);*/

    // Making a camera matrix and then inverting it 
    DirectX::SimpleMath::Matrix camera = DirectX::XMMatrixLookToRH({ 0, 0, 0 }, playerForward, { 0, 1, 0 });

    // Pushing the model forward in the current view direction
    DirectX::SimpleMath::Matrix offset = (DirectX::SimpleMath::Matrix::CreateTranslation(playerTranslation.Translation() + playerForward));

    // Multiplying all the matrices into one
    DirectX::SimpleMath::Matrix result = m_mInfo.rot * m_mInfo.trans * m_mInfo.scale * camera.Invert() * offset;

    renderInfo.transform = result;
}

const WeaponModel::WeaponModelAnimationInfo& Logic::WeaponModel::getModelInfo() const
{
    return m_mInfo;
}

void WeaponModel::render() const
{
    RenderQueue::get().queue(&renderInfo);
}
