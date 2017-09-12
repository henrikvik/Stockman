#ifndef SKILLMANAGER_H
#define SKILLMANAGER_H
#pragma region ClassDesc
		/*
			CLASS: SkillManager
			AUTHOR:

			DESCRIPTION: This class is made to manage the skills of the system.
		*/
#pragma endregion

#include "Skill.h"
#include <vector>

namespace Logic
{
	class SkillManager
	{
	private:
		std::vector<Skill> m_skills;
	public:
		void init();

		void switchSkill();
		void useSkill();

		void update();
	};
}
#endif
