#ifndef UPGRADE_H
#define UPGRADE_H

#pragma region Comment

/*
	Author: Fredholm
	This class keeps track on upgrades
*/

#pragma endregion Description of class

#include <vector>
#include "Effect.h"

namespace Logic
{
	enum UPGRADE_FLAG {
		UPGRADE_IS_WEAPON			= 0x1,
		UPGRADE_INCREASE_DMG		= 0x2,
		UPGRADE_INCREASE_CD			= 0x4,
		UPGRADE_INCREASE_SIZE		= 0x8,
		UPGRADE_INCREASE_AMMOCAP	= 0x20,
		UPGRADE_INCREASE_MAGSIZE	= 0x40,
		UPGRADE_IS_BOUNCING			= 0x80,
		UPGRADE_TRANSFERABLE		= 0x100,
	};

	class Upgrade
	{
	public:
		Upgrade();
		Upgrade(const Upgrade& other) = delete;
		Upgrade* operator=(const Upgrade& other) = delete;
		~Upgrade();

		// Add get & sets here when you're not lazy

	private:
		long long			m_register;
		int					m_actionID;				//< The specific ID of the weapon or skill

		// Flat upgrades
		float				m_increaseDmg;
		float				m_increaseCooldown;
		float				m_increaseSize;
		float				m_increaseAmmoCap;
		float				m_increaseMagSize;

		std::vector<Effect> m_tranferEffects;		//< The list of effects that should be tranfered on hit
	};
}

#endif // !UPGRADE_H
