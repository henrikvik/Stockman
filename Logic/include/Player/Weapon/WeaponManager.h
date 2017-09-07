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
	private:
		std::vector<std::pair<Weapon, Weapon>> m_weapons;
	public:
		void usewWaponPrimary();
		void useWeaponSecondary();
	};
}
#endif
