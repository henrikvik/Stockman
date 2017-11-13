#include <AI/Trigger.h>
using namespace Logic;

Trigger::Trigger(Resources::Models::Files modelID, btRigidBody* body, btVector3 halfExtent, float cooldown, bool reusable)
: Entity(body, halfExtent)
{
	m_maxCooldown = cooldown;
	m_cooldown = -1;
	m_active = true;
	m_reusable = reusable;
	m_remove = false;

    animatedRenderInfo.model = modelID;
//    animatedRenderInfo.animationName = "Rotation";
//    animatedRenderInfo.animationProgress = 0;
    animatedRenderInfo.transform = getTransformMatrix();
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

// Affects
void Trigger::affect(int stacks, Effect const & effect, float deltaTime)
{

}

// Checks if the trigger is non-active, if so, update the cooldown
void Trigger::updateSpecific(float deltaTime)
{
    // Update trigger animation
//    animatedRenderInfo.animationProgress += deltaTime;

    // Update resuability
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
void Trigger::onCollision(PhysicsObject& other, btVector3 contactPoint, float dmgMultiplier)
{
	if (m_active)
	{
		if (Player* p = dynamic_cast<Player*>(&other))
		{
			// Sending statuses over to player
			for (StatusManager::UPGRADE_ID u : getStatusManager().getActiveUpgrades())
				p->getStatusManager().addUpgrade(u);
			for (std::pair<int, StatusManager::EFFECT_ID> effect : getStatusManager().getActiveEffectsIDs())
				p->getStatusManager().addStatus(effect.second, effect.first, true);

			if (m_reusable)
			{
				// Starting Cooldown
				m_cooldown = m_maxCooldown;
				m_active = false;
			}
			else
			{
				// Remove this trigger
				m_remove = true;
			}
		}
	}
}

bool Trigger::getShouldRemove() const
{
	return m_remove;
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

void Trigger::setShouldRemove(bool remove)
{
	m_remove = remove;
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

void Trigger::render() const
{
    RenderQueue::get().queue(animatedRenderInfo);
}
