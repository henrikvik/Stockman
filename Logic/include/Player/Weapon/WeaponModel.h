#ifndef WEAPON_MODEL_H
#define WEAPON_MODEL_H

#include <Entity\Object.h>
#include <Graphics/include/RenderQueue.h>

namespace Logic
{
    class WeaponModel
    {
    public:

        struct WeaponModelAnimationInfo
        {
            DirectX::SimpleMath::Matrix rot, trans, scale;
            float swapTimer;
        };

        WeaponModel();
        WeaponModel(Resources::Models::Files modelID, WeaponModelAnimationInfo mInfo);
        ~WeaponModel();

        void update(float deltaTime, DirectX::SimpleMath::Matrix playerTranslation, DirectX::SimpleMath::Vector3 playerForward);

        const WeaponModelAnimationInfo& getModelInfo() const;

        void render() const;

    private:
        DirectX::SimpleMath::Matrix m_current;
        WeaponModelAnimationInfo m_mInfo;
        //PlayerRenderInfo renderInfo;
        StaticRenderInfo renderInfo; // temp
    };
}

#endif // !WEAPON_MODEL_H

