#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H
#pragma region ClassDesc
		/*
			CLASS: ActionManager
			AUTHOR:

			DESCRIPTION: This class is made to manage the weapons and skills by calling the weapons and skills respective managers.
		*/
#pragma endregion

#include "Skill/SkillManager.h"
#include "Weapon/WeaponManager.h"

namespace Logic
{
	class ActionManager
	{
	public:
		ActionManager();
		ActionManager(const ActionManager& other) = delete;
		ActionManager* operator=(const ActionManager& other) = delete;
		~ActionManager();

		void init();

		void update(float deltaTime);

		// Weapon
		void switchToPrimary();
		void switchToSecondary();
		void useWeapon();

		// Skill
		void switchSkill();
		void useSkill();

	private:
		SkillManager m_skillManager;
		WeaponManager m_weaponManager;
	};
}

#endif