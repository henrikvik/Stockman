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
		void update(float deltaTime);

		void addStatus(int statusID, int nrOfStacks, bool resetDuration);
		void removeOneStatus(int statusID);
		void removeAllStatus(int statusID);

		std::vector<Upgrade*> getUpgrades();
	private:
		/* BASELINE EFFECTS */
		static const int NR_OF_EFFECTS = 2;
		static Effect s_effects[NR_OF_EFFECTS];

		struct EffectStack {
			int stack;
			float duration;
		};

		std::vector<EffectStack> m_effectStacks; // fast loop speed bad lookup, but worth it? :<
		std::vector<int> m_effectStacksIds; // mike acton approved (i hop)

		void removeEffect(int index);

		// int = id
		std::vector<Upgrade*> m_upgrades;
	};
}


#endif
