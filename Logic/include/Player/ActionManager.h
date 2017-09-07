#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include "Skill/SkillManager.h"
#include "Weapon/WeaponManager.h"

namespace Logic
{
	class ActionManager
	{
	private:
		SkillManager m_skillManager;
		WeaponManager m_weaponManager;
	public:
		void update();
	};
}

#endif