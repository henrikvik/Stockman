#ifndef SKILLBULLETTIME_H
#define SKILLBULLETTIME_H
#pragma region ClassDesc
		/*
			CLASS: SkillBulletTime
			AUTHOR:

			DESCRIPTION: TODO
		*/
#pragma endregion

#include <Player\Skill\Skill.h>
#include <Misc\GameTime.h>

namespace Logic
{
	class SkillBulletTime : public Skill
	{
	private:
		GameTime* m_gameTime;
	public:
		SkillBulletTime(GameTime* gameTime);

		void onUse();
		void onUpdate(float deltaTime);
		void render(Graphics::Renderer& renderer);
	};
}
#endif