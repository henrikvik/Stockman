#ifndef AMMO_CONTAINER_H
#define AMMO_CONTAINER_H

namespace Logic
{
    class AmmoContainer
    {
    public:
        struct AmmoInfo
        {
            int enhancedAmmoCap;
            int enhancedAmmo;
            int magSize;
            int magAmmo;
            int ammoConsumption[2];
            int enhancedAmmoConsumption[2];
            float reloadTime;
        };

        AmmoContainer();
        AmmoContainer(AmmoInfo ammoInfo);
        ~AmmoContainer();

        void reset();

        const AmmoContainer::AmmoInfo& getAmmoInfo() const;

        bool removeAmmo(int weapon);
        void setEnhancedAmmo(int ammo);
        void fillMag(float modifier);

    private:
        AmmoInfo m_aInfo;

    };
}
#endif // !AMMO_CONTAINER_H


