#ifndef SKILLMANAGER_H
#define SKILLMANAGER_H
#pragma region ClassDesc
		/*
			CLASS: SkillManager
			AUTHOR:

			DESCRIPTION: This class is made to manage the skills of the system.
		*/
#pragma endregion

#include <Misc\GameTime.h>
#include <vector>
#include <d3d11.h>
#include <SimpleMath.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

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
		GameTime* m_gameTime;	

		std::vector<Skill*> m_allSkills;
  //      std::pair<Skill*> m_currentSkill;
		Skill* m_currentSkill;
	};
}
#endif
