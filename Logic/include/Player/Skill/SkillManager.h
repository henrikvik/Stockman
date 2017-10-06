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

		void init(Physics* physics, ProjectileManager* projectileManager, GameTime* gameTime);
		void clear();

		void switchToSkill(int index);

		void useSkill(btVector3 forward, Entity& shooter);
		void releaseSkill();

		void update(float deltaTime);
		void render(Graphics::Renderer& renderer);

		bool getCanBeUsed() const;

        Skill* getCurrentSkill() const;
	
	private:
		bool m_canBeUsed;
		GrapplingHookState m_state;
		ProjectileManager* m_projectileManager;	
		GameTime* m_gameTime;	

		std::vector<Skill*> m_allSkills;
		Skill* m_currentSkill;
	};
}
#endif
