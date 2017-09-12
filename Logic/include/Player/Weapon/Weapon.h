#ifndef WEAPON_H
#define WEAPON_H

#include "../../Projectile/ProjectileStruct.h"

#pragma region ClassDesc
		
		/*
			CLASS: Weapon
			AUTHOR:

			DESCRIPTION: This class defines a weapon of the system
		*/

#pragma endregion

namespace Logic
{
	class Weapon
	{
	private:
		int m_weaponID;
		int m_ammoCap;
		int m_ammo;
		int m_magSize;
		int m_magAmmo;
		float m_damage;
		float m_attackRate;
		float m_freeze;
		float m_reloadTime;
		ProjectileData m_projectileData;
	//	Animation m_animation;
	public:
		Weapon();
		Weapon(int weaponID, int ammoCap, int ammo, int magSize, int magAmmo, float damage, float attackRate, float freeze, float reloadTime);

		void use();

		void update();

		int getAmmoCap();
		void setAmmoCap(int ammoCap);
		int getAmmo();
		void setAmmo(int ammo);
		int getMagSize();
		void setMagSize(int magSize);
		int getMagAmmo();
		void removeMagAmmo();
		void removeMagAmmo(int ammo);

		void fillMag();
	};
}

#endif