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

namespace Logic
{
	class SkillShieldCharge : public Skill
	{
	private:
		/*ProjectileData m_projectileData;*/
	public:
		void onUse();
		void onUpdate(float deltaTime);
		void render(Graphics::Renderer& renderer);
	};
}
#endif