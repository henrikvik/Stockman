#ifndef WEAPON_MODEL_H
#define WEAPON_MODEL_H

#include <Entity\Object.h>

namespace Logic
{
    class WeaponModel : public Object
    {
    public:
        struct WeaponModelAnimationInfo
        {
            DirectX::SimpleMath::Matrix rot, trans, scale;
            float swapTimer;
        };

        WeaponModel();
        WeaponModel(Graphics::ModelID modelID, WeaponModelAnimationInfo mInfo);
        ~WeaponModel();

        void update(DirectX::SimpleMath::Matrix playerTranslation, DirectX::SimpleMath::Vector3 playerForward);

        const WeaponModelAnimationInfo& getModelInfo() const;

    private:
        WeaponModelAnimationInfo m_mInfo;
    };
}

#endif // !WEAPON_MODEL_H

