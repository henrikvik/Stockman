#ifndef SKILLGRAPPLINGHOOK_H
#define SKILLGRAPPLINGHOOK_H
#pragma region ClassDesc
		/*
			CLASS: SkillGrapplingHook
			AUTHOR:

			DESCRIPTION: TODO
		*/
#pragma endregion

#include <Player\Skill\Skill.h>

namespace Logic
{
	class SkillGrapplingHook : public Skill
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