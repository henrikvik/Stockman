#include "Player/Skill/SkillBulletTime.h"

using namespace Logic;

SkillBulletTime::SkillBulletTime(ProjectileManager* projectileManager, ProjectileData pData)
: Skill(BULLET_TIME_CD)
{
	m_projectileData = pData;
	m_projectileManager = projectileManager;
	m_sensor = nullptr;
	//m_travelProjectile = nullptr;
}

void SkillBulletTime::onUse(btVector3 forward, Entity& shooter)
{
	printf("Bullet Time used.\n");
	m_sensor = m_projectileManager->addProjectile(m_projectileData, shooter.getPositionBT(), forward, shooter);

	btRigidBody* bodySensor = m_sensor->getRigidBody();

	bodySensor->setLinearVelocity(btVector3(0.f, 0.f, 0.f));
	bodySensor->setCollisionFlags(bodySensor->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

	/*ProjectileData travelPData = m_projectileData;
	travelPData.scale = 0.0001f;
	travelPData.type = ProjectileType::ProjectileTypeBulletTime;
	m_travelProjectile = m_projectileManager->addProjectile(travelPData, shooter.getPositionBT(), forward, shooter);*/
}

void SkillBulletTime::onRelease() { }

void SkillBulletTime::onUpdate(float deltaTime)
{
	/*if (m_sensor)
		if (m_sensor->shouldRemove())
			m_sensor = nullptr;
	if (m_travelProjectile)
		if (m_travelProjectile->shouldRemove())
			m_travelProjectile = nullptr;

	if (m_sensor && m_travelProjectile)
	{
		m_sensor->getRigidBody()->setWorldTransform(m_travelProjectile->getTransform());
	}*/
}

void SkillBulletTime::render(Graphics::Renderer& renderer)
{

}
