#ifndef SKILL_H
#define SKILL_H

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
