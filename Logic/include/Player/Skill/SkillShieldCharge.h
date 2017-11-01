#ifndef SKILLSHIELDCHARGE_H
#define SKILLSHIELDCHARGE_H

#pragma region ClassDesc
		/*
			CLASS: SkillShieldCharge
			AUTHOR:

			DESCRIPTION: TODO
		*/
#pragma endregion

#include <Player\Skill\Skill.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

namespace Logic
{
    class Entity;

	class SkillShieldCharge : public Skill
	{
	private:
		bool m_active, m_slowdown;
		float m_time;
		btVector3 m_forw;
		Entity *m_shooter;
		float m_chargePower;
	public:
		SkillShieldCharge();
		~SkillShieldCharge();

        bool onUse(btVector3 forward, Entity& shooter);
		void onRelease();
		void onUpdate(float deltaTime);
		void render();
	};
}
#endif
