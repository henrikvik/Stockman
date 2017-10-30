#include <Player\Weapon\AmmoContainer.h>

using namespace Logic;

AmmoContainer::AmmoContainer()
{
    m_aInfo = AmmoInfo();
}

AmmoContainer::AmmoContainer(AmmoInfo ammoInfo)
{
    m_aInfo = ammoInfo;
}

AmmoContainer::~AmmoContainer()
{
}

void Logic::AmmoContainer::reset()
{
    m_aInfo.ammo = m_aInfo.ammoCap;
    m_aInfo.magAmmo = m_aInfo.magSize;
}

AmmoContainer::AmmoInfo& AmmoContainer::getAmmoInfo()
{
    return m_aInfo;
}

void AmmoContainer::removePrimaryAmmo()
{
    if (m_aInfo.primAmmoConsumption > m_aInfo.magAmmo)
        m_aInfo.magAmmo = 0;
    else
        m_aInfo.magAmmo -= m_aInfo.primAmmoConsumption;
}

void AmmoContainer::removeSecondaryAmmo()
{
    if (m_aInfo.secAmmoConsumption > m_aInfo.magAmmo)
        m_aInfo.magAmmo = 0;
    else
        m_aInfo.magAmmo -= m_aInfo.secAmmoConsumption;
}

void AmmoContainer::fillMag()
{
    int toAdd = m_aInfo.magSize - m_aInfo.magAmmo;

    if (m_aInfo.ammo >= toAdd)
    {
        m_aInfo.ammo -= toAdd;		// Remove ammo from total
        m_aInfo.magAmmo = m_aInfo.magSize;	// Add ammo to mag
    }
    else
    {
        m_aInfo.magAmmo += m_aInfo.ammo;	// Add rest of ammo to mag
        m_aInfo.ammo = 0;				// Remove rest of ammo from total
    }
}