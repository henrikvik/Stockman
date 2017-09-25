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

void TriggerManager::addTrigger(Physics& physics, std::vector<StatusManager::UPGRADE_ID> upgrades, std::vector<StatusManager::EFFECT_ID> effects)
{
	Trigger* trigger = new Trigger(physics.createBody(Cube({ 5, 1, 5 }, { 0, 0 ,0 }, { 5, 5, 5 }), 0.f, true), {5, 5, 5}, 1.f);

	if (!upgrades.empty())
		trigger->addUpgrades(upgrades);

	if (!effects.empty())
		trigger->addEffects(effects);

	m_triggers.push_back(trigger);
}

void TriggerManager::update(float deltaTime) 
{
	for (Trigger* t : m_triggers)
		t->update(deltaTime);
}

void TriggerManager::render(Graphics::Renderer & renderer)
{
	for (Trigger* trigger : m_triggers)
		trigger->render(renderer);
}

std::vector<Trigger*>& Logic::TriggerManager::getTriggers()
{
	return m_triggers;
}
