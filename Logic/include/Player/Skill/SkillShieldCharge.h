#ifndef SKILLSHIELDCHARGE_H
#define SKILLSHIELDCHARGE_H
#pragma region ClassDesc
		/*
			CLASS: SkillShieldCharge
			AUTHOR:

			DESCRIPTION: TODO
		*/
#pragma endregion
#include "Skill.h"

namespace Logic
{
	class SkillShieldCharge : public Skill
	{
	private:
		/*ProjectileData m_projectileData;*/
	public:
		void update();
	};
}
#endif