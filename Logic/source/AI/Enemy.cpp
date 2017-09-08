#include <AI\Enemy.h>
using namespace Logic;

Enemy::Enemy(float health, float baseDamage, int enemyType, int animationId) {
	m_health = health;
	m_baseDamage = baseDamage;
	m_enemyType = enemyType;

	//animation todo
}

Enemy::~Enemy() {

}

void Enemy::update(float deltaTime) {
	updateSpec(deltaTime);
}