#ifndef EFFECT_H
#define EFFECT_H

#pragma region Comment

/*

	Author: Fredholm
	This class keeps track on effects

*/

#pragma endregion Description of class

namespace Logic
{
	typedef enum EFFECT_FLAG {
		EFFECT_KILL					= 0x1,
		EFFECT_ON_FIRE				= 0x2,
		EFFECT_IS_FROZEN			= 0x4, 
		EFFECT_MODIFY_DMG_GIVEN		= 0x8,
		EFFECT_MODIFY_DMG_TAKEN		= 0x20,
		EFFECT_MODIFY_HP			= 0x40,
		EFFECT_MODIFY_FIRERATE		= 0x80,
		EFFECT_MODIFY_MOVEMENTSPEED	= 0x100,
		EFFECT_MODIFY_FREEZING		= 0x200,
		EFFECT_MODIFY_BULLET_TIME	= 0x400,
	};

	class Effect
	{
	public:
		Effect();
		Effect(const Effect& other) = delete;
		Effect* operator=(const Effect& other) = delete;
		~Effect();

		// Add get & sets here when you're not lazy

	private:
		long long flags;
		float	duration;		//< Duration of this effect

		// Modifiers
		float	m_modifyDmgGiven;
		float	m_modifyDmgTaken;
		float	m_modifyHP;
		float	m_modifyFirerate;
		float	m_modifyMovementSpeed;

		// Specific 
		float	m_isFreezing;	//< The amount of freezed (0 : 1)
		float	m_isBulletTime;	//< Multiplier of the current bulletTime (for example, this effect will adjust every entity except player during the bullet time skill)
	};
}

#endif // !EFFECT_H