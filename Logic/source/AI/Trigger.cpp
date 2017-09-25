#include <AI/Trigger.h>
using namespace Logic;

Trigger::Trigger(btRigidBody* body, btVector3 halfExtent, float cooldown)
: Entity(body, halfExtent)
{
	m_maxCooldown = cooldown;
	m_cooldown = -1;
	m_active = true;
}

Trigger::~Trigger() { }

// Adds a vector of upgrades for this trigger (mostly ammo-pickups)
void Trigger::addUpgrades(const std::vector<StatusManager::UPGRADE_ID>& upgrades)
{
	for (StatusManager::UPGRADE_ID uID : upgrades)
		this->getStatusManager().addUpgrade(uID);

}

// Adds a vector of effects for this trigger
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
		// Sending statuses over to player
		for (StatusManager::UPGRADE_ID u : getStatusManager().getActiveUpgrades())
			other.getStatusManager().addUpgrade(u);
		for (std::pair<int, StatusManager::EFFECT_ID> effect: getStatusManager().getActiveEffectsIDs())
			other.getStatusManager().addStatus(effect.second, effect.first, true);

		// Starting Cooldown
		m_cooldown = m_maxCooldown;
		m_active = false;
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