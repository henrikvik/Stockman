#include <Player\HUDHandler.h>
#include <Player\Player.h>
#include <Player\Skill\Skill.h>
#include <Player\Skill\SkillManager.h>

#include <AI\WaveTimeManager.h>
#include <AI\EntityManager.h>

#include <Misc\ComboMachine.h>

#include <Graphics\include\Renderer.h>

using namespace Logic;

HUDHandler::HUDHandler()
{
    info = new Graphics::HUDInfo;
}


HUDHandler::~HUDHandler()
{
    delete info;
}

void HUDHandler::update(Player const &player, WaveTimeManager const &timeManager,
    EntityManager const &manager)
{
    //updates hudInfo with the current info
    info->score = ComboMachine::Get().GetCurrentScore();
    info->hp = player.getHP();
    info->activeAmmo[0] = player.getActiveWeapon()->getMagAmmo();
    info->activeAmmo[1] = player.getActiveWeapon()->getAmmo();
    info->inactiveAmmo[0] = player.getActiveWeapon()->getMagAmmo();
    info->inactiveAmmo[1] = player.getActiveWeapon()->getAmmo();
    if (player.isUsingMeleeWeapon())
    {
        info->sledge = true;
    }

    // HUD info on the first skill
    Skill* primary = player.getSkill(SkillManager::ID::PRIMARY);
    if (!primary->getCanUse())
        info->cd = primary->getCooldown() / primary->getCooldownMax();
    else
        info->cd = 1.0f;

}

void HUDHandler::render(Graphics::Renderer &renderer)
{
    renderer.fillHUDInfo(info);
}
