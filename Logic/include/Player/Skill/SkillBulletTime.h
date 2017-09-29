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

#define BULLET_TIME_CD 2500.f

namespace Logic
{
	class SkillBulletTime : public Skill
	{
	private:
		GameTime* m_gameTime;
	public:
		SkillBulletTime(float cooldown, GameTime* gameTime);

		void onUse(btVector3 forward, Entity& shooter);
		void onUpdate(float deltaTime);
		void render(Graphics::Renderer& renderer);
	};
}
#endif