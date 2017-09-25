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

namespace Logic
{
	class Skill
	{
	private:
		// StatusManager statusManager;
		float m_cooldown;
		float m_duration;
	public:
		void use();
		virtual void onUse() = 0;
		
		void update(float deltaTime);
		virtual void onUpdate(float deltaTime) = 0;

		virtual void render(Graphics::Renderer& renderer) = 0;
	};
}
#endif
