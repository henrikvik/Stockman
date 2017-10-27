#include "Player/Skill/SkillBulletTime.h"

using namespace Logic;

SkillBulletTime::SkillBulletTime(ProjectileManager* projectileManager, ProjectileData pData)
	: Skill(BULLET_TIME_CD, BULLET_TIME_DURATION)
{
	m_projectileData = pData;
	m_projectileManager = projectileManager;
	m_sensor = nullptr;
	//m_travelProjectile = nullptr;
}

bool SkillBulletTime::onUse(btVector3 forward, Entity& shooter)
{
    setCanUse(false);
    setCooldown(BULLET_TIME_CD);

	printf("Bullet Time used.\n");
	m_sensor = m_projectileManager->addProjectile(m_projectileData, shooter.getPositionBT(), forward, shooter);

	btRigidBody* bodySensor = m_sensor->getRigidBody();

	bodySensor->setLinearVelocity(btVector3(0.f, 0.f, 0.f));
	bodySensor->setCollisionFlags(bodySensor->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

	slowDownIntervals.clear();
	speedUpIntervals.clear();
	for (int i = 1; i < BULLET_TIME_SMOOTHNESS_INTERVAL; i++)
	{
		slowDownIntervals.push_back(i * ((BULLET_TIME_SLOW_DOWN_DURATION) / BULLET_TIME_SMOOTHNESS_INTERVAL) + BULLET_TIME_DURATION - BULLET_TIME_SLOW_DOWN_DURATION);
		speedUpIntervals.push_back(i * (BULLET_TIME_SPEED_UP_DURATION / BULLET_TIME_SMOOTHNESS_INTERVAL));
	}

	m_stacks = 0;

    return true;

	/*ProjectileData travelPData = m_projectileData;
	travelPData.scale = 0.0001f;
	travelPData.type = ProjectileType::ProjectileTypeBulletTime;
	m_travelProjectile = m_projectileManager->addProjectile(travelPData, shooter.getPositionBT(), forward, shooter);*/
}

void SkillBulletTime::onRelease() { }

void SkillBulletTime::onUpdate(float deltaTime)
{
	if (m_sensor)
	{
		if (m_sensor->getProjectileData().ttl < deltaTime)
			m_sensor = nullptr;
		else
		{
			if (!slowDownIntervals.empty() && slowDownIntervals.back() > getDuration())
			{
				m_stacks++;
				slowDownIntervals.pop_back();
			}
			else if (!speedUpIntervals.empty() && speedUpIntervals.back() > getDuration())
			{
				m_stacks--;
				speedUpIntervals.pop_back();
			}

			if (m_stacks != 0)
				m_sensor->getStatusManager().addStatus(StatusManager::EFFECT_ID::BULLET_TIME, m_stacks);
		}
	}

	/*if (m_travelProjectile)
		if (m_travelProjectile->shouldRemove())
			m_travelProjectile = nullptr;

	if (m_sensor && m_travelProjectile)
	{
		m_sensor->getRigidBody()->setWorldTransform(m_travelProjectile->getTransform());
	}*/
}

void SkillBulletTime::render(Graphics::Renderer& renderer)
{
	if (m_sensor && m_sensor->getProjectileData().ttl > 0)
		renderer.setBulletTimeCBuffer(m_sensor->getProjectileData().ttl / (float)BULLET_TIME_DURATION);
}
