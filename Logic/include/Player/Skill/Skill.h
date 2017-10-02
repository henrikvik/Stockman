#ifndef SKILL_H
#define SKILL_H
#pragma region ClassDesc
		/*
			CLASS: Skill
			AUTHOR:

			DESCRIPTION: This class defines a skill of the system
		*/
#pragma endregion

#include <Graphics\include\Renderer.h>
#include <Entity\Entity.h>

namespace Logic
{
	class Skill
	{
	public:
		Skill(float cooldown, float duration = 0.f);

		void use(btVector3 forward, Entity& shooter);
		virtual void onUse(btVector3 forward, Entity& shooter) = 0;

		void release();
		virtual void onRelease() = 0;
		
		void update(float deltaTime);
		virtual void onUpdate(float deltaTime) = 0;

		virtual void render(Graphics::Renderer& renderer) = 0;

		float	getCooldown() const;
		float	getCooldownMax() const;
		float	getDuration() const;
		bool	getCanUse() const;

	private:
		// StatusManager statusManager;
		float	m_cooldown;
		float	m_cooldownMax;
		float	m_duration;
		bool	m_canUse;
	};
}
#endif
