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
		enum EFFECT_ID {
			ON_FIRE, FREEZE
		};

		enum UPGRADE_ID {
			BOUNCE, P10_AMMO
		};

		StatusManager();
		~StatusManager();

		void clear();
		void update(float deltaTime);

		void addStatus(int statusID, int nrOfStacks, bool resetDuration);
		void removeOneStatus(int statusID);
		void removeAllStatus(int statusID);

		void addUpgrade(UPGRADE_ID id);

		std::vector<Upgrade>* getUpgrades();
	private:
		/* BASELINE EFFECTS */
		struct EffectStack {
			int stack;
			float duration;
		};

		static const int NR_OF_EFFECTS = 2, NR_OF_UPGRADES = 2;
		static Effect s_effects[NR_OF_EFFECTS];
		static Upgrade s_upgrades[NR_OF_UPGRADES];

		// m_effectStacksIds[i] = id of the effect at m_effectsStacks[i]
		std::vector<EffectStack> m_effectStacks; // fast loop speed bad lookup, but worth it? :<
		std::vector<int> m_effectStacksIds; // mike acton approved (i hop)

		void removeEffect(int index);

		std::vector<Upgrade> m_upgrades;
	};
}


#endif
