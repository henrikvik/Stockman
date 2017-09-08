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
		float m_damage;
		float m_attackRate;
		float m_freeze;
		ProjectileData m_projectileData;
	//	Animation m_animation;
	public:
		Weapon();
		Weapon(int weaponID, int ammoCap, int magSize, float damage, float attackRate, float freeze);

		void use();

		void update();
	};
}

#endif