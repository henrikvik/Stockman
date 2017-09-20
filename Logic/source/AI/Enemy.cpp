#include <AI\Enemy.h>
using namespace Logic;

Enemy::Enemy(btRigidBody* body, btVector3 halfExtent, float health, float baseDamage, int enemyType, int animationId)
: Entity(body, halfExtent)
{
	m_health = health;
	m_baseDamage = baseDamage;
	m_enemyType = enemyType;

	//animation todo
}

Enemy::~Enemy() {

}

void Enemy::update(float deltaTime) {
	updateSpecific(deltaTime);
}

void Enemy::damage(float damage)
{
	m_health -= damage;
}

void Enemy::affect(int stacks, Effect const &effect, float dt) 
{
	int flags = effect.getStandards()->flags;

	if (flags & Effect::EFFECT_KILL)
		damage(m_health);
	if (flags & Effect::EFFECT_ON_FIRE)
		damage(effect.getModifiers()->modifyDmgTaken * dt);
}

float Enemy::getHealth() const
{
	return m_health;
}

float Enemy::getMaxHealth() const
{
	return m_maxHealth;
}

float Enemy::getBaseDamage() const
{
	return m_baseDamage;
}

int Enemy::getEnemyType() const
{
	return m_enemyType;
}