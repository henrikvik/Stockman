#include "Player/Skill/SkillGrapplingHook.h"

using namespace Logic;

SkillGrapplingHook::SkillGrapplingHook(Physics* physics)
: Skill(GRAPPLING_HOOK_CD)
{
	m_physicsPtr = physics;
}

SkillGrapplingHook::~SkillGrapplingHook()
{
	m_physicsPtr = nullptr;
}

void SkillGrapplingHook::onUse(btVector3 forward, Entity& shooter)
{


}

void SkillGrapplingHook::onUpdate(float deltaTime)
{

}

void SkillGrapplingHook::render(Graphics::Renderer& renderer)
{

}