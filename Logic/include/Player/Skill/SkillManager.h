#ifndef SKILLMANAGER_H
#define SKILLMANAGER_H
#pragma region ClassDesc
		/*
			CLASS: SkillManager
			AUTHOR:

			DESCRIPTION:
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
		void update();
	};
}
#endif
