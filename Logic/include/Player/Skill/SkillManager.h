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

        // Easier to understand which is which in "player.cpp"
        struct ID
        {
            static const int PRIMARY = 0;
            static const int SECONDARY = 1;
            static const int TERTIARY = 2;
        };

        struct THRESHOLD
        {
            static const int MAX_SKILLS = 3;
        };

		SkillManager();
		~SkillManager();

		void init(Physics* physics, ProjectileManager* projectileManager, GameTime* gameTime);
		void clear();

		void switchToSkill(std::vector<int> skillsToUse);

		void use(int index, btVector3 forward, Entity& shooter);
		void release(int index);

		void update(float deltaTime);
		void render(Graphics::Renderer& renderer);

        Skill* getSkill(int index) const;
	
	private:

        bool isLegit(int index) const;

		std::vector<Skill*> m_allSkills;
        Skill* m_current[THRESHOLD::MAX_SKILLS];
	};
}
#endif
