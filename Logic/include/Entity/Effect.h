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
			EFFECT_KILL					        = 0x1,
			EFFECT_ON_FIRE				        = 0x2,
			EFFECT_IS_FROZEN			        = 0x4, 
			EFFECT_MODIFY_DMG_GIVEN		        = 0x8,
			EFFECT_MODIFY_DMG_TAKEN		        = 0x10,
			EFFECT_MODIFY_HP			        = 0x20,
			EFFECT_MODIFY_FIRERATE		        = 0x40,
			EFFECT_BOUNCE	                    = 0x80,
			EFFECT_MODIFY_FREEZING		        = 0x100,
			EFFECT_MODIFY_AMMO	                = 0x200,
			EFFECT_CONSTANT_INVINC		        = 0x400,
			EFFECT_CONSTANT_PUSH_BACK	        = 0x800,
			EFFECT_CONSTANT_DAMAGE_ON_CONTACT   = 0x1000,
			EFFECT_BULLET_TIME			        = 0x2000,
            EFFECT_IS_STUNNED                   = 0x4000,
            EFFECT_MOVE_FASTER                  = 0x8000,
            EFFECT_MOVE_SLOWER                  = 0x10000,
            EFFECT_IS_WEAPON                    = 0x20000,
            EFFECT_IS_SKILL                     = 0x40000,
            EFFECT_CROSSBOW                     = 0X80000,
            EFFECT_ICESTAFF                     = 0x100000,
            EFFECT_SLEDGEHAMMER                 = 0x200000,
            EFFECT_DECREASE_CD                  = 0x400000,
            EFFECT_INCREASE_SIZE                = 0x800000,
            EFFECT_INCREASE_AMMOCAP             = 0x1000000,
            EFFECT_INCREASE_MAGSIZE             = 0x2000000,
            EFFECT_INCREASE_MOVEMENTSPEED       = 0x4000000,
            EFFECT_INCREASE_FIRERATE            = 0x8000000,
            EFFECT_DECREASE_RELOADTIME          = 0x10000000,
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
            int     modifyAmmoCap;
            int     modifyMagCap;
            float   modifySkillCDDecrease;
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