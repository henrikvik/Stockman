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
            float forwardMultiplier;
        };

        WeaponModel();
        WeaponModel(Graphics::ModelID modelID, WeaponModelInfo mInfo);
        ~WeaponModel();

        void update(DirectX::SimpleMath::Matrix playerTranslation, DirectX::SimpleMath::Vector3 playerForward);

    private:
        WeaponModelInfo m_mInfo;
    };
}

#endif // !WEAPON_MODEL_H

