#include <AI/Enemy.h>
using namespace Logic;

Enemy::Enemy(float health, float baseDamage, int enemyType, int animationId) {

}

Enemy::~Enemy() {

}

void Enemy::update(float deltaTime) {
	updateSpec(deltaTime);
}