#ifndef SKILLMANAGER_H
#define SKILLMANAGER_H
#pragma region ClassDesc
		/*
			CLASS: SkillManager
			AUTHOR:

			DESCRIPTION: This class is made to manage the skills of the system.
		*/
#pragma endregion

#include <Graphics\include\Renderer.h>
#include <Player\Skill\SkillBulletTime.h>
#include <Player\Skill\SkillGrapplingHook.h>
#include <Player\Skill\SkillShieldCharge.h>
#include <Projectile\ProjectileManager.h>
#include <Misc\GameTime.h>
#include <vector>

namespace Logic
{
	class SkillManager
	{

	public:
		SkillManager();
		~SkillManager();

		void init(ProjectileManager* projectileManager, GameTime* gameTime);
		void clear();

		void switchToSkill(int index);

		void useSkill();
		void update(float deltaTime);
		void render(Graphics::Renderer& renderer);
	
	private:
		void initializeSkills(GameTime* gameTime);
		
		ProjectileManager* projectileManager;
		GameTime* gameTime;

		std::vector<Skill*> m_allSkills;
		Skill* m_currentSkill;
	};
}
#endif
