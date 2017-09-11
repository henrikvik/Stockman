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
		enum EFFECT_LIST {
			ON_FIRE, FREEZE
		};

		StatusManager();
		StatusManager(const StatusManager& other) = delete;
		StatusManager* operator=(const StatusManager& other) = delete;
		~StatusManager();

		void clear();
		void addStatus(int statusID);
		void removeOneStatus(int statusID);
		void removeAllStatus(int statusID);

		std::vector<std::pair<int, Effect*>> getEffects();
		std::vector<Upgrade*> getUpgrades();

	private:
		/* BASELINE EFFECTS */
		static Effect s_onFire, s_freeze;
		static Effect *s_effects[2]; // same size as nr of effects (above)

		// int = id
		std::vector<std::pair<int, Effect*>> m_effects;
		std::vector<Upgrade*> m_upgrades;
	};
}


#endif // !STATUSMANAGER_H
