#ifndef SKILL_H
#define SKILL_H
#pragma region ClassDesc
		/*
			CLASS: Skill
			AUTHOR:

			DESCRIPTION:
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
