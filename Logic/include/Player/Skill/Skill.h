#ifndef SKILL_H
#define SKILL_H
#pragma region ClassDesc
		/*
			CLASS: Skill
			AUTHOR:

			DESCRIPTION: This class defines a skill of the system
		*/
#pragma endregion

namespace Logic
{
	class Skill
	{
	private:
		float m_cooldown;
		float m_duration;
	public:
		void update();
	};
}
#endif
