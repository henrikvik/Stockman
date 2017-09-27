#include <AI/TriggerManager.h>
using namespace Logic;

TriggerManager::TriggerManager() 
{
}

TriggerManager::~TriggerManager()  
{
	for (Trigger* trigger : m_triggers)
		delete trigger;
	m_triggers.clear();
}

void TriggerManager::removeTrigger(Trigger * t, int index)
{
	m_physicsPtr->removeRigidBody(t->getRigidbody());
	t->destroyBody();
	delete t;
	m_triggers.erase(m_triggers.begin() + index);
}

// Adds a trigger, with certain cooldown & buffs, (cooldown is is ms)
void TriggerManager::addTrigger(Cube& cube, float cooldown, Physics& physics, std::vector<StatusManager::UPGRADE_ID> upgrades, std::vector<StatusManager::EFFECT_ID> effects, bool reusable)
{
	this->m_physicsPtr = &physics;

	Trigger* trigger = new Trigger(physics.createBody(cube, TRIGGER_MASS, TRIGGER_IS_SENSOR), cube.getDimensions(), cooldown, reusable);

	if (!upgrades.empty())
		trigger->addUpgrades(upgrades);

	if (!effects.empty())
		trigger->addEffects(effects);

	m_triggers.push_back(trigger);
}

// Updates all the triggers cooldowns
void TriggerManager::update(float deltaTime) 
{
	std::vector<Trigger*> removeThese;
	for (size_t i = 0; i < m_triggers.size(); i++)
	{
		Trigger* t = m_triggers[i];
		t->update(deltaTime);
		
		// Remove triggers
		if (t->getShouldRemove())
			removeTrigger(t, i);
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
