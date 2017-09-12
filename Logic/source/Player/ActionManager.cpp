#include "Player/ActionManager.h"

using namespace Logic;

ActionManager::ActionManager()
{

}

ActionManager::~ActionManager() { }

void ActionManager::init()
{
//	m_skillManager.init();
	m_weaponManager.init();
}


void ActionManager::clear()
{
//	m_skillManager.clear();
	m_weaponManager.clear();
}

void ActionManager::switchWeapon()
{
}

void ActionManager::useWeapon()
{

}

void ActionManager::switchSkill()
{

}

void ActionManager::useSkill()
{

}

void ActionManager::update(float deltaTime)
{
//	m_weaponManager.update(deltaTime);
//	m_skillManager.update(deltaTime);
}
