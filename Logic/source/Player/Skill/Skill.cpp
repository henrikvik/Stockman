#include "Player/Skill/Skill.h"

using namespace Logic;

void Skill::use()
{
	onUse();
}

void Skill::update(float deltaTime)
{
	onUpdate(deltaTime);
}
