#include <AI/Trigger.h>
using namespace Logic;

Trigger::Trigger(btRigidBody* body, btVector3 halfExtent, float cooldown)
: Entity(body, halfExtent)
{
	m_cooldown = cooldown;
	m_active = (m_cooldown < NULL) ? true : false;

	this->getStatusManager().addUpgrade(StatusManager::UPGRADE_ID::P10_AMMO);
	this->getStatusManager().addStatus(StatusManager::EFFECT_ID::MOVEMENT_SPEED, 1, true);
}

Trigger::~Trigger() { }

void Trigger::addUpgrades(const std::vector<StatusManager::UPGRADE_ID>& upgrades)
{
	for (StatusManager::UPGRADE_ID uID : upgrades)
		this->getStatusManager().addUpgrade(uID);

}

void Trigger::addEffects(const std::vector<StatusManager::EFFECT_ID>& effects)
{
	for (StatusManager::EFFECT_ID eID : effects)
		this->getStatusManager().addStatus(eID, 1, true);
}

// Checks if the trigger is non-active, if so, update the cooldown
void Trigger::update(float deltaTime) 
{
	if (!m_active)
	{
		m_cooldown -= deltaTime;
		if (m_cooldown < NULL)
		{
			m_active = true;
		}
	}
}

// Collision with the player, give player the effect
void Trigger::onCollision(Entity& other)
{
	if (m_active)
	{
		if (Player* p = dynamic_cast<Player*>(&other))
		{
			for (StatusManager::UPGRADE_ID u : getStatusManager().getActiveUpgrades())
				p->getStatusManager().addUpgrade(u);
			for (std::pair<int, StatusManager::EFFECT_ID> effect: getStatusManager().getActiveEffectsIDs())
				p->getStatusManager().addStatus(effect.second, effect.first, true);
		}
	}
}

bool Trigger::getIsActive() const
{
	return m_active;
}

bool Trigger::getIsReusable() const
{
	return m_reusable;
}

float Trigger::getCooldown() const
{
	return m_cooldown;
}

void Trigger::setIsActive(bool active)
{
	m_active = active;
}

void Trigger::setIsReusable(bool reusable)
{
	m_reusable = reusable;
}

void Trigger::setCooldown(float cooldown) 
{
	m_cooldown = cooldown;
}