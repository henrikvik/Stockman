#include "Player/Skill/SkillShieldCharge.h"

using namespace Logic;

SkillShieldCharge::SkillShieldCharge(ProjectileManager * projectileManager, ProjectileData projectileData)
: Skill(SHIELD_CHARGE_CD, SHIELD_CHARGE_DURATION)
{
	m_projectileManager = projectileManager;
	m_projectileData = projectileData;
	active = false;
	time = 0.0f;
}

SkillShieldCharge::~SkillShieldCharge()
{
	m_projectileManager = nullptr;
}

void SkillShieldCharge::onUse(btVector3 forward, Entity& shooter)
{
	printf("Used Shield Charge.\n");
	active = true;
	forw = forward * SHIELD_CHARGE_DURATION;
	thePlayer = &shooter;
	thePlayer->getRigidbody()->applyCentralImpulse(forw);
}

void SkillShieldCharge::onUpdate(float deltaTime)
{
	if (active == true)
	{
		if (time >= SHIELD_CHARGE_DURATION)
		{
			btVector3 stop(0.0f, 0.0f, 0.0f);
			active == false;
			time = 0;
		}
		time += deltaTime;
	}
	//Move you and bullet on a set speed for a set amount of time
	//deactivate the bool
}

void SkillShieldCharge::render(Graphics::Renderer& renderer)
{

}