#ifndef SKILLGRAPPLINGHOOK_H
#define SKILLGRAPPLINGHOOK_H
#pragma region ClassDesc
		/*
			CLASS: SkillGrapplingHook
			AUTHOR:

			DESCRIPTION:
		*/
#pragma endregion
#include "Skill.h"
namespace Logic
{
	class SkillGrapplingHook : public Skill
	{
	private:
		/*ProjectileData m_projectileData;*/
	public:
		void update();
	};
}
#endif