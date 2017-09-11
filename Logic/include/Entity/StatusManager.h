#ifndef STATUSMANAGER_H
#define STATUSMANAGER_H

#pragma region Comment

/*
	Author: Fredholm / Lukas

	This class keeps track on upgrades for both weapons and skills
*/

#pragma endregion Description of class

#include <vector>
#include "Effect.h"
#include "Upgrade.h"

namespace Logic
{
	class StatusManager
	{
	public:
		/* STANDARDS EFFECTS */
		static Effect onFire, freeze;
		/* BASELINE EFFECTS */

		StatusManager();
		StatusManager(const StatusManager& other) = delete;
		StatusManager* operator=(const StatusManager& other) = delete;
		~StatusManager();

		void clear();
		void addStatus(int statusID);
		void removeStatus(int statusID);

		std::vector<Effect*> getEffects();
		std::vector<Upgrade*> getUpgrades();

	private:
		std::vector<Effect*> m_effects;
		std::vector<Upgrade*> m_upgrades;
	};
}


#endif // !STATUSMANAGER_H
