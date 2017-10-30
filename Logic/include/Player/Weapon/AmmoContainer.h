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
            int ammoCap;
            int ammo;
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

        AmmoContainer::AmmoInfo& getAmmoInfo();

        void removePrimaryAmmo();
        void removeSecondaryAmmo();
        void fillMag();

    private:
        AmmoInfo m_aInfo;

    };
}
#endif //AMMO_CONTAINER_H


