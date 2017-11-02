#include <AI/TriggerManager.h>
#include <Physics\Physics.h>
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
Trigger* TriggerManager::addTrigger(Graphics::ModelID modelID, Cube& cube, float cooldown, Physics& physics, std::vector<StatusManager::UPGRADE_ID> upgrades, std::vector<StatusManager::EFFECT_ID> effects, bool reusable)
{
	this->m_physicsPtr = &physics;

	Trigger* trigger = new Trigger(modelID, physics.createBody(cube, TRIGGER_MASS, TRIGGER_IS_SENSOR), cube.getDimensions(), cooldown, reusable);

	if (!upgrades.empty())
		trigger->addUpgrades(upgrades);

	if (!effects.empty())
		trigger->addEffects(effects);

	m_triggers.push_back(trigger);
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
void TriggerManager::render(Graphics::Renderer & renderer)
{
	for (Trigger* t : m_triggers)
		t->render(renderer);
}

std::vector<Trigger*>& Logic::TriggerManager::getTriggers()
{
	return m_triggers;
}
