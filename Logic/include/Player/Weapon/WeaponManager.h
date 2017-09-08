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

		void switchWeapon(int weaponID);

		void useSecondary();
		void usePrimary();

	private:

		void initializeWeapons();
		void makeWeaponLoadout();

		std::vector<Weapon> m_allWeapons;
		std::vector<std::pair<Weapon*, Weapon*>> m_weaponsLoadouts;
		std::pair<Weapon*, Weapon*> m_currentWeapon;
	};
}
#endif
