#ifndef WEAPONMANAGER_H
#define WEAPONMANAGER_H
#pragma region ClassDesc
		/*
			CLASS: WeaponManager
			AUTHOR:

			DESCRIPTION: This class is made to manage the weapons of the system.
		*/
#pragma endregion

#include <vector>
#include "Weapon.h"

namespace Logic
{
	

	class WeaponManager
	{

	public:		
		WeaponManager();
		WeaponManager(const WeaponManager& other) = delete;
		WeaponManager* operator=(const WeaponManager& other) = delete;
		~WeaponManager();

		// Creates every weapon 
		void init();
		void makeWeaponLoadout();

		void switchToPrimary();
		void switchToSecondary();

		void useCurrentWeapon();

	private:
		std::vector<Weapon> m_AllWeapons;
		std::vector<std::pair<Weapon, Weapon>> m_weapons;
	};
}
#endif
