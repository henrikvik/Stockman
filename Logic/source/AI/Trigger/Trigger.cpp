#include <AI/Trigger/Trigger.h>

#include <Player\Player.h>
#include <Entity\StaticObject.h>
#include <Misc\Sound\NoiseMachine.h>

using namespace Logic;

Trigger::Trigger(Resources::Models::Files modelID, btRigidBody* body, btVector3 halfExtent, TriggerType type, float cooldown, bool reusable)
: Entity(body, halfExtent)
{
    m_type = type;
	m_maxCooldown = cooldown;
	m_cooldown = -1;
	m_active = true;
	m_reusable = reusable;
	m_remove = false;

    // Setting up Animation information
    animatedRenderInfo.model = modelID;
//    animatedRenderInfo.animationName = "Rotation";
//    animatedRenderInfo.animationProgress = 0;
    animatedRenderInfo.transform = getTransformMatrix();
    animatedRenderInfo.cull_radius = 5.f;
    
    // Setting up Light information
    light.position = DirectX::SimpleMath::Vector3(body->getWorldTransform().getOrigin());
    light.color = DirectX::SimpleMath::Vector4(1.f, 1.f, 1.f, 1.f);
    light.range = 1.f;
    light.intensity = 1.f;
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
		this->getStatusManager().addStatusResetDuration(eID, 1);
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

    updateSpecificType(deltaTime);

    animatedRenderInfo.transform = getTransformMatrix();
}

// Collision with the player, give player the effect
void Trigger::onCollision(PhysicsObject& other, btVector3 contactPoint, float dmgMultiplier)
{
	if (m_active)
	{
        if (Player* player = dynamic_cast<Player*>(&other)) { onCollisionPlayer(*player); }
        else if (StaticObject* terrain = dynamic_cast<StaticObject*>(&other)) { onCollisionTerrain(*terrain); }
	}
}

void Trigger::onCollisionPlayer(Player& player)
{
    // Sending statuses over to player
    player.getStatusManager().copyUpgradesFrom(getStatusManager());
    for (std::pair<int, StatusManager::EFFECT_ID> effect : getStatusManager().getActiveEffectsIDs())
        player.getStatusManager().addStatus(effect.second, effect.first);

    // Plays unique sound effect
    switch (m_type)
    {
        case TriggerType::JUMPPAD:              Sound::NoiseMachine::Get().playSFX(Sound::SFX::TRIGGER_JUMPPAD, nullptr, true);                                 break;
        case TriggerType::AMMO_PICKUP_BOLT:     Sound::NoiseMachine::Get().playSFX(Sound::SFX::TRIGGER_PICKUP, nullptr, true); player.setPickupTintTimer(0.2f); break;
        case TriggerType::AMMO_PICKUP_CRYSTAL:  Sound::NoiseMachine::Get().playSFX(Sound::SFX::TRIGGER_PICKUP, nullptr, true); player.setPickupTintTimer(0.2f); break;
        case TriggerType::KILL_DEATH_ZONE:      Sound::NoiseMachine::Get().playSFX(Sound::SFX::HELLO, nullptr, true);                                           break;
    }

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

void Trigger::onCollisionTerrain(StaticObject& terrain)
{
    if (hasCallback(ON_COLLISION))
    {
        CallbackData data;
        data.caller = this;
        data.dataPtr = reinterpret_cast<std::intptr_t> (&terrain);
        this->callback(ON_COLLISION, data);
    }
}

Trigger::TriggerType Trigger::getType() const
{
    return m_type;
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
    QueueRender(animatedRenderInfo);
    QueueRender(light);
}
