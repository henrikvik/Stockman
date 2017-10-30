#ifndef WEAPON_MODEL_H
#define WEAPON_MODEL_H

#include <Entity\Object.h>

namespace Logic
{
    class WeaponModel : public Object
    {
    public:
        struct WeaponModelInfo
        {
            DirectX::SimpleMath::Matrix rot, trans, scale;
        };

        WeaponModel();
        WeaponModel(Graphics::ModelID modelID, WeaponModelInfo mInfo, float forwardMultiplier);
        ~WeaponModel();

        void update(DirectX::SimpleMath::Matrix playerTranslation, DirectX::SimpleMath::Vector3 playerForward);

    private:
        WeaponModelInfo m_mInfo;
        float m_forwardMultiplier;
    };
}

#endif // !WEAPON_MODEL_H

