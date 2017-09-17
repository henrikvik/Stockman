#include <AI\Enemy.h>
using namespace Logic;

Enemy::Enemy(btRigidBody* body, float health, float baseDamage, int enemyType, int animationId)
: Entity(body)
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