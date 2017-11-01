#include <Player\HUDManager.h>
#include <Player\Player.h> 
#include <Player\Skill\Skill.h>
#include <Player\Skill\SkillManager.h>
#include <Player\Weapon\Weapon.h>
#include <Player\Weapon\AmmoContainer.h>

#include <AI\WaveTimeManager.h>
#include <AI\EntityManager.h>

#include <Misc\ComboMachine.h>

#include <Graphics\include\Renderer.h>

using namespace Logic;

const int HUDManager::CURRENT_AMMO = 0;
const int HUDManager::TOTAL_AMMO = 1;

HUDManager::HUDManager()
{
    info = newd Graphics::HUDInfo;
    ZeroMemory(info, sizeof(info));
    info->cd = 1.0f;
}

HUDManager::~HUDManager()
{
    delete info;
}

void HUDManager::update(Player const &player, WaveTimeManager const &timeManager,
    EntityManager const &entityManager)
{
    //updates hudInfo with the current info
    info->score = ComboMachine::Get().GetCurrentScore();
    info->hp = player.getHP();
    info->activeAmmo[HUDManager::CURRENT_AMMO] =     player.getActiveAmmoContainer().getAmmoInfo().magAmmo;
    info->activeAmmo[HUDManager::TOTAL_AMMO] = player.getActiveAmmoContainer().getAmmoInfo().ammo;
    info->inactiveAmmo[HUDManager::CURRENT_AMMO] = player.getInactiveAmmoContainer().getAmmoInfo().magAmmo;
    info->inactiveAmmo[HUDManager::TOTAL_AMMO] = player.getInactiveAmmoContainer().getAmmoInfo().ammo;
    info->sledge = player.isUsingMeleeWeapon();

    // HUD info on the first skill
    const Skill* primary = player.getSkill(SkillManager::ID::PRIMARY);
    if (!primary->getCanUse())
        info->cd = primary->getCooldown() / primary->getCooldownMax();
    else
        info->cd = 1.0f;

    info->wave = timeManager.getCurrentWave() + 1;
    info->timeRemaining = (timeManager.getTimeRequired() - timeManager.getTimeCurrent()) * 0.001f;
    info->enemiesRemaining = (int)entityManager.getNrOfAliveEnemies();
}

void HUDManager::render(Graphics::Renderer &renderer)
{
    renderer.fillHUDInfo(info);
}