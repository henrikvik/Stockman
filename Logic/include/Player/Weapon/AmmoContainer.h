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

        const AmmoContainer::AmmoInfo& getAmmoInfo() const;

        void removePrimaryAmmo();
        void removeSecondaryAmmo();
        void setAmmo(int ammo);
        void fillMag();

    private:
        AmmoInfo m_aInfo;

    };
}
#endif //AMMO_CONTAINER_H


