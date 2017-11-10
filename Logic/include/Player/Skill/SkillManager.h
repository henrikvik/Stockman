#ifndef SKILLMANAGER_H
#define SKILLMANAGER_H
#pragma region ClassDesc
		/*
			CLASS: SkillManager
			AUTHOR:

			DESCRIPTION: This class is made to manage the skills of the system.
		*/
#pragma endregion

#include <d3d11.h>
#include <SimpleMath.h>

#include <vector>
#include <btBulletCollisionCommon.h>


namespace Graphics
{
    class Renderer;
}

namespace Logic
{
    class Physics;
    class Entity;
    class ProjectileManager;
    class Skill;
    enum GrapplingHookState;

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

        enum SKILL
        {
            SKILL_GRAPPLING_HOOK,
            SKILL_SHIELD_CHARGE,
            SKILL_BULLET_TIME
        };

		SkillManager();
		~SkillManager();

		void init(Physics* physics, ProjectileManager* projectileManager);

		void switchToSkill(std::vector<SKILL> skillsToUse);

		void use(int index, btVector3 forward, Entity& shooter);
		void release(int index);

		void update(float deltaTime);
		void render();

        Skill* getSkill(int index) const;
	
	private:

        bool isLegit(int index) const;

		std::vector<Skill*> m_allSkills;
        __int16 m_nrOfSkills;
        Skill* m_current[THRESHOLD::MAX_SKILLS];
	};
}
#endif
