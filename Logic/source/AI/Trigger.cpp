#include <AI/Trigger.h>
using namespace Logic;

Trigger::Trigger(btRigidBody* body, btVector3 halfExtent, float cooldown)
: Entity(body, halfExtent)
{
	m_cooldown = cooldown;
}

Trigger::~Trigger() {

}

void Trigger::update(float deltaTime) {
	m_cooldown -= deltaTime;
}

void Trigger::setCooldown(float cooldown) {

}

float Trigger::getCooldown() const {
    return 0.0f;
}