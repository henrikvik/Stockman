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

void Enemy::affect(int stacks, Effect const &effect) {
	long long flags = effect.getStandards()->flags;

	if (flags & Effect::EFFECT_KILL)
		m_health = 0;
}