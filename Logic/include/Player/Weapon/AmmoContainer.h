#ifndef AMMO_CONTAINER_H
#define AMMO_CONTAINER_H

namespace Logic
{
    class AmmoContainer
    {
    public:
        struct AmmoInfo
        {
            // AmmoType ammoType; probably?
            //int ammoCap;
            //int ammo;
            int enhancedAmmoCap;
            int enhancedAmmo;
            int magSize;
            int magAmmo;
            int primAmmoConsumption;
            int secAmmoConsumption;
            float reloadTime;
        };

        AmmoContainer();
        AmmoContainer(AmmoInfo ammoInfo);
        ~AmmoContainer();

        void reset();

        const AmmoContainer::AmmoInfo& getAmmoInfo() const;

        bool removePrimaryAmmo();
        bool removeSecondaryAmmo();
        void setEnhancedAmmo(int ammo);
        void fillMag(int modifier);

    private:
        AmmoInfo m_aInfo;

    };
}
#endif // !AMMO_CONTAINER_H


