#include <Player\Weapon\WeaponModel.h>

using namespace Logic;

WeaponModel::WeaponModel()
{
}

WeaponModel::WeaponModel(Graphics::ModelID modelID, WeaponModelInfo mInfo, float forwardMultiplier)
    : Object(modelID)
{
    m_mInfo = mInfo;
    m_forwardMultiplier = forwardMultiplier;
}


WeaponModel::~WeaponModel()
{
}

void Logic::WeaponModel::update(DirectX::SimpleMath::Matrix playerTranslation, DirectX::SimpleMath::Vector3 playerForward)
{
    static DirectX::SimpleMath::Matrix camera, result, offset;

    // Making a camera matrix and then inverting it 
    camera = DirectX::XMMatrixLookToRH({ 0, 0, 0 }, playerForward, { 0, 1, 0 });

    // Pushing the model forward in the current view direction
    offset = (DirectX::SimpleMath::Matrix::CreateTranslation(playerTranslation.Translation() + playerForward * m_forwardMultiplier));

    // Multiplying all the matrices into one
    result = m_mInfo.rot * m_mInfo.trans * m_mInfo.scale * camera.Invert() * offset;

    this->setWorldTranslation(result);
}