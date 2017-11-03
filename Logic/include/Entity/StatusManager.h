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
		/* BASELINE EFFECTS */
		struct EffectStack {
			int stack;
			float duration;
		};

		enum EFFECT_ID {
			ON_FIRE, FREEZE, BOOST_UP, AMMO_PICK_UP_PRIMARY, AMMO_PICK_UP_SECONDARY,
            SHIELD_CHARGE, BULLET_TIME, ENRAGE, HEALTH_P1, LAST_ITEM_IN_EFFECTS
		};

		enum UPGRADE_ID {
			BOUNCE, P10_AMMO, LAST_ITEM_IN_UPGRADES
		};

		StatusManager();
		~StatusManager();

		void clear();
		void update(float deltaTime);

		void addStatus(StatusManager::EFFECT_ID effect_id, int nrOfStacks, bool resetDuration = false);
		void removeOneStatus(int statusID);
		void removeAllStatus(int statusID);

		void addUpgrade(UPGRADE_ID id);
		Upgrade& getUpgrade(UPGRADE_ID id);

		const Effect& getEffect(EFFECT_ID id) const;

		/* returns 0 if effect is not active
		this is O(n) BUT it should NEVER be called ONCE PER FRAME
		use affect() then instead 
		
		Don
		*/
		int getStacksOfEffectFlag(Effect::EFFECT_FLAG flag) const;

        // Returns true if upgrade is active
        bool isOwningUpgrade(Upgrade::UPGRADE_FLAG flag);

		// nr of stacks and the effect itself
		std::vector<std::pair<int, Effect*>> getActiveEffects();
		// return stack and id of effect
		std::vector<std::pair<int, StatusManager::EFFECT_ID>> getActiveEffectsIDs();
		std::vector<UPGRADE_ID>& getActiveUpgrades();
	private:
		static const int NR_OF_EFFECTS = EFFECT_ID::LAST_ITEM_IN_EFFECTS, NR_OF_UPGRADES = UPGRADE_ID::LAST_ITEM_IN_UPGRADES;
		static Effect s_effects[NR_OF_EFFECTS];
		static Upgrade s_upgrades[NR_OF_UPGRADES];

		// m_effectStacksIds[i] = id of the effect at m_effectsStacks[i]
		std::vector<EffectStack> m_effectStacks; // fast loop speed bad lookup, but worth it? :<
		std::vector<StatusManager::EFFECT_ID> m_effectStacksIds; // mike acton approved (i hop)

		void removeEffect(int index);

		std::vector<UPGRADE_ID> m_upgrades;
	};
}


#endif
