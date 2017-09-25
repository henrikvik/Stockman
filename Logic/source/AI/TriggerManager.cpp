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

// Adds a trigger, with certain cooldown & buffs, (cooldown is is ms)
void TriggerManager::addTrigger(Cube& cube, float cooldown, Physics& physics, std::vector<StatusManager::UPGRADE_ID> upgrades, std::vector<StatusManager::EFFECT_ID> effects)
{
	Trigger* trigger = new Trigger(physics.createBody(cube, TRIGGER_MASS, TRIGGER_IS_SENSOR), cube.getDimensions(), cooldown);

	if (!upgrades.empty())
		trigger->addUpgrades(upgrades);

	if (!effects.empty())
		trigger->addEffects(effects);

	m_triggers.push_back(trigger);
}

// Updates all the triggers cooldowns
void TriggerManager::update(float deltaTime) 
{
	for (Trigger* t : m_triggers)
		t->update(deltaTime);
}

// Draws all the triggers
void TriggerManager::render(Graphics::Renderer & renderer)
{
	for (Trigger* trigger : m_triggers)
		trigger->render(renderer);
}

std::vector<Trigger*>& Logic::TriggerManager::getTriggers()
{
	return m_triggers;
}
