#include <AI/Trigger/TriggerManager.h>
#include <Physics\Physics.h>
#include <AI\Trigger\TriggerAmmoPickup.h>

using namespace Logic;

TriggerManager::TriggerManager() 
{
}

TriggerManager::~TriggerManager()  
{
    reset();
}

// it added reset so i can do it beetwen playes :)
void TriggerManager::reset()
{
    for (Trigger* trigger : m_triggers)
    {
        m_physicsPtr->removeRigidBody(trigger->getRigidBody());
        trigger->destroyBody();
        delete trigger;
    }
    m_triggers.clear();
}

void TriggerManager::removeTrigger(Trigger * t, int index)
{
	m_physicsPtr->removeRigidBody(t->getRigidBody());
	t->destroyBody();
	delete t;
	m_triggers.erase(m_triggers.begin() + index);
}

// Adds a trigger, with certain cooldown & buffs, (cooldown is is ms)
Trigger* TriggerManager::addTrigger(Trigger::TriggerType type, btVector3 const &pos, Physics& physics, std::vector<StatusManager::UPGRADE_ID> upgrades, std::vector<StatusManager::EFFECT_ID> effects)
{
    m_physicsPtr = &physics; // ?

    Trigger* trigger = nullptr;

    switch (type)
    {
    case Trigger::TriggerType::JUMPPAD:
        trigger = newd Trigger(Resources::Models::Jump_Pad, 
            physics.createBody(Cube(pos, { 0, 0, 0 }, { 2, 0.1f, 2 }),
            TRIGGER_MASS, TRIGGER_IS_SENSOR), { 0.5, 0.5, 0.5f },
            type, 500.f, true);
        break;
    case Trigger::TriggerType::AMMO_PICKUP_BOLT:
        trigger = newd TriggerAmmoPickup(Resources::Models::AmmoPackCrossBolt,
            physics.createBody(Cube(pos, { 0, 0, 0 }, { 1.f, 1.f, 1.f }),
            TRIGGER_MASS, TRIGGER_IS_SENSOR), { 1.f, 1.f, 1.f },
            type, 0.f, false);
        break;
    case Trigger::TriggerType::AMMO_PICKUP_CRYSTAL:
        trigger = newd TriggerAmmoPickup(Resources::Models::Ammocrystal,
            physics.createBody(Cube(pos, { 0, 0, 0 }, { 1.f, 1.f, 1.f }),
            TRIGGER_MASS, TRIGGER_IS_SENSOR), { 1.f, 1.f, 1.f },
            type, 0.f, false);
        break;
    case Trigger::TriggerType::KILL_DEATH_ZONE: // doesnt work
        trigger = newd Trigger(Resources::Models::Staff,
            physics.createBody(Cube(pos, { 0, 0, 0 }, { 900., 51.f, 900.f }),
                TRIGGER_MASS, TRIGGER_IS_SENSOR), { 1.f, 1.f, 1.f },
            type, 0.f, true);
        break;
    default:
        trigger = nullptr;
    }

    if (trigger)
    {
        if (!upgrades.empty())
            trigger->addUpgrades(upgrades);

        if (!effects.empty())
            trigger->addEffects(effects);

        m_triggers.push_back(trigger);
    }
    return trigger;
}

// Updates all the triggers cooldowns
void TriggerManager::update(float deltaTime) 
{
	std::vector<Trigger*> removeThese;
	for (size_t i = 0; i < m_triggers.size(); i++)
	{
		Trigger* t = m_triggers[i];
		t->updateSpecific(deltaTime);
		
		// Remove triggers
		if (t->getShouldRemove())
			removeTrigger(t, (int)i);
	}
}

// Draws all the triggers
void TriggerManager::render() const
{
	for (Trigger* t : m_triggers)
		t->render();
}

std::vector<Trigger*>& Logic::TriggerManager::getTriggers()
{
	return m_triggers;
}
