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

		void switchToSkill(std::pair<int, int> index);

		void usePrimarySkill(btVector3 forward, Entity& shooter);
        void useSecondarySkill(btVector3 forward, Entity& shooter);

		void releasePrimarySkill();
        void releaseSecondarySkill();

		void update(float deltaTime);
		void render(Graphics::Renderer& renderer);

        Skill* getPrimarySkill() const;
        Skill* getSecondarySkill() const;
	
	private:

		std::vector<Skill*> m_allSkills;
        std::pair<Skill*, Skill*> m_current;
	};
}
#endif
