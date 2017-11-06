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
	class Effect
	{
	public:
		enum EFFECT_FLAG {
			EFFECT_KILL					= 0x1,
			EFFECT_ON_FIRE				= 0x2,
			EFFECT_IS_FROZEN			= 0x4, 
			EFFECT_MODIFY_DMG_GIVEN		= 0x8,
			EFFECT_MODIFY_DMG_TAKEN		= 0x10,
			EFFECT_MODIFY_HP			= 0x20,
			EFFECT_MODIFY_FIRERATE		= 0x40,
			EFFECT_MODIFY_MOVEMENTSPEED	= 0x80,
			EFFECT_MODIFY_FREEZING		= 0x100,
			EFFECT_MODIFY_AMMO	        = 0x200,
			EFFECT_CONSTANT_INVINC		= 0x400,
			EFFECT_CONSTANT_PUSH_BACK	= 0x800,
			EFFECT_CONSTANT_DAMAGE_ON_CONTACT = 0x1000,
			EFFECT_BULLET_TIME			= 0x2000,
            EFFECT_IS_STUNNED           = 0x4000,
		};

		struct Standards {
			long long flags;
			float	duration;		//< Duration of this effect (0 is instant like +hp or something)
		};

		struct Modifiers {
			float	modifyDmgGiven;
			float	modifyDmgTaken;
			float	modifyHP;
			float	modifyFirerate;
			float	modifyMovementSpeed;
		};

		struct Specifics {
			float	isFreezing;	//< The amount of freezed (0 : 1)
			float	isBulletTime;	//< Multiplier of the current bulletTime (for example, this effect will adjust every entity except player during the bullet time skill)
            float   ammoType;
		};

		Effect();
		Effect(Standards const &standards);
		Effect(Effect const &other);

		Effect* operator=(Effect const &other);
		~Effect();

		Standards* getStandards() const;
		Modifiers* getModifiers() const;
		Specifics* getSpecifics() const;

		void setStandards(Standards const &standards);
		void setModifiers(Modifiers const &modifiers);
		void setSpecifics(Specifics const &specifics);
	private:
		Standards *m_standards;
		Modifiers *m_modifiers;
		Specifics *m_specifics;
	};
}

#endif // !EFFECT_H