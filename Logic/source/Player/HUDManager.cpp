#include <Player\HUDManager.h>
#include <Player\Player.h> 
#include <Player\Skill\Skill.h>
#include <Player\Skill\SkillManager.h>
#include <Player\Weapon\Weapon.h>

#include <AI\WaveTimeManager.h>
#include <AI\EntityManager.h>

#include <Misc\ComboMachine.h>



#include <comdef.h>

using namespace Logic;



const int HUDManager::CURRENT_AMMO = 0;
const int HUDManager::TOTAL_AMMO = 1;

//constructs the spriteRenderInfos that the hud consists of
void Logic::HUDManager::constructGUIElements()
{
    //crosshair


    float crosshairSize = 50;
    HUDElements.push_back(Sprite( Sprite::CENTER, Sprite::CENTER, 0, 0, crosshairSize, crosshairSize, Resources::Textures::Crosshair, FloatRect({ 0.0f, 0.0f }, { 1.f, 1.f })));

    //crossbow
    float x = 3.0f / 1024;
    float y = 517.0f / 1024;
    float width = 503.0f / 1024;
    float height = 503.0f / 1024;
    HUDElements.push_back(Sprite(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -65, -60, 110, 110, Resources::Textures::weaponsheet, FloatRect({ x, y }, {x + width, y + height })));

    //ice staff

    x = 4.0f / 1024;
    y = 5.0f / 1024;
    width = 503.0f / 1024;
    height = 503.0f / 1024;

    HUDElements.push_back(Sprite(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -65, -175, 110, 110, Resources::Textures::weaponsheet, FloatRect({ x, y }, { x + width, y + height })));

    //sledegehammer
    x = 519.0f / 1024;
    y = 9.0f / 1024;
    width = 503.0f / 1024;
    height = 503.f / 1024;

    HUDElements.push_back(Sprite(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -132, -25, 75, 75, Resources::Textures::weaponsheet, FloatRect({ x, y }, { x + width, y + height })));

    //creates the weapon mask
    x = 518.0f / 1024;
    y = 519.0f / 1024;
    width = 503.0f / 1024;
    height = 503.f / 1024;
    HUDElements.push_back(Sprite(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -65, -60, 110, 110, Resources::Textures::weaponsheet, FloatRect({ x, y }, { x + width, y + height })));
    


    //creates the difrent skill icons 

    //grappling 
    x = 519.0f / 1024;
    y = 9.0f / 1024;
    width = 503.0f / 1024;
    height = 503.f / 1024;
    skillList.push_back(Sprite(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -60, -148, 50, 50, Resources::Textures::Spellsheet, FloatRect({ x, y }, { x + width, y + height })));

    //charge 
    x = 3.0f / 1024;
    y = 517.0f / 1024;
    width = 503.0f / 1024; 
    height = 503.0f / 1024;
    skillList.push_back(Sprite(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -60, -148, 50, 50, Resources::Textures::Spellsheet, FloatRect({ x, y }, { x + width, y + height })));
    

    //bullet time 
    x = 4.0f / 1024;
    y = 5.0f / 1024;
    width = 503.0f / 1024;
    height = 503.0f / 1024;
    skillList.push_back(Sprite(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -60, -148, 50, 50, Resources::Textures::Spellsheet, FloatRect({ x, y }, { x + width, y + height })));


    //skill mask 1
    x = 518.0f / 1024;
    y = 519.0f / 1024;
    width = 503.0f / 1024;
    height = 503.f / 1024;
    skillMasks.push_back(Sprite(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -60, -148, 50, 50, Resources::Textures::Spellsheet, FloatRect({ x, y }, { x + width, y + height })));

    //skill mask 2
    x = 518.0f / 1024;
    y = 519.0f / 1024;
    width = 503.0f / 1024;
    height = 503.f / 1024;
    skillMasks.push_back(Sprite(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -130, -148, 50, 50, Resources::Textures::Spellsheet, FloatRect({ x, y }, { x + width, y + height })));


    
 

   

    for (size_t i = 0; i < PLAYER_STARTING_HP; i++)
    {
        x = 26.f / 1024;
        y = 258.0f / 1024;
        width = 311.0f / 1024;
        height = 171.0f / 1024;
        HPBar.push_back(Sprite(Sprite::BOTTOM_LEFT, Sprite::BOTTOM_LEFT, 100 + (i * 45), -90, 40, 35, Resources::Textures::Gamesheet, FloatRect({ x, y }, {x + width, y + height })));

        x = 26.f / 1024;
        y = 454.0f / 1024;
        width = 311.0f / 1024;
        height = 171.0f / 1024;
        staticElements.push_back(Sprite(Sprite::BOTTOM_LEFT, Sprite::BOTTOM_LEFT, 100 + (i * 45), -90, 40, 35, Resources::Textures::Gamesheet, FloatRect({ x, y }, { x + width, y + height })));
    }


    //static hud elemets

    //shield thing by hp
    x = 374.f / 1024;
    y = 457.0f / 1024;
    width = 162.0f / 1024;
    height = 268.0f / 1024;
    staticElements.push_back(Sprite(Sprite::BOTTOM_LEFT, Sprite::BOTTOM_LEFT, 45, -60, 50, 70, Resources::Textures::Gamesheet, FloatRect({ x, y }, { x + width, y + height })));

    //score banner
    x = 26.f / 1024;
    y = 23.0f / 1024;
    width = 364.0f / 1024;
    height = 85.0f / 1024;
    staticElements.push_back(Sprite(Sprite::TOP_LEFT, Sprite::TOP_LEFT, 60, 15, 170, 30, Resources::Textures::Gamesheet, FloatRect({ x, y }, { x + width, y + height })));

    //wave counter
    x = 26.f / 1024;
    y = 143.0f / 1024;
    width = 508.0f / 1024;
    height = 85.0f / 1024;
    staticElements.push_back(Sprite(Sprite::TOP_LEFT, Sprite::TOP_LEFT, 505, 15, 200, 30, Resources::Textures::Gamesheet, FloatRect({ x, y }, { x + width, y + height })));

    //wave timer
    x = 559.f / 1024;
    y = 143.0f / 1024;
    width = 161.0f / 1024;
    height = 85.0f / 1024;
    staticElements.push_back(Sprite(Sprite::TOP_LEFT, Sprite::TOP_LEFT, 720, 15, 70, 30, Resources::Textures::Gamesheet, FloatRect({ x, y }, { x + width, y + height })));

}

//updates the active weapons and cd icons
void Logic::HUDManager::updateGUIElemets()
{
    //hp

    if (HPBar.size() != info.hp && info.hp != 0)
    {
        HPBar.pop_back();
    }

    //decideds active weapon 
    switch (info.currentWeapon)
    {
        
    case 0:
        //crossbow
        HUDElements.at(WEAPONMASK).setScreenPos(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -65, -60, 110, 110);
        break;
        
    case 1:
        //icestaff
        HUDElements.at(WEAPONMASK).setScreenPos(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -65, -175, 110, 110);
        break;
        
    case 2:
        //sledgehammer
        HUDElements.at(WEAPONMASK).setScreenPos(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -132, -25, 75, 75);
        break;
    default:
        break;
    }


    //skills
    if (skillChoosen == false && info.currentSkills[0] >= 0 && info.currentSkills[1] >= 0)
    {
        
        HUDElements.push_back(skillList.at(info.currentSkills[0]));
        HUDElements.push_back(skillList.at(info.currentSkills[1]));
        HUDElements.at(SKILL2).setScreenPos(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -130, -148, 50, 50);

        skillChoosen = true;
    }
    else
    {
        if (info.currentSkills[0] >= 0 && 1.0f - info.cd[0] > FLT_EPSILON)
        {
            skillMasks.at(0).setTopYToProcent(1 - info.cd[0], 519.0f / 1024, 0.725000024f);
        }
        

        if (info.currentSkills[1] >= 0 && 1.0f - info.cd[1] > FLT_EPSILON)
        {
            skillMasks.at(1).setTopYToProcent(1 - info.cd[1], 519.0f / 1024, 0.725000024f);
        }

    }
}


HUDManager::HUDManager()
{

    info.cd[0] = 1.0f;
    info.cd[1] = 1.0f;
    info.currentSkills[0] = -1;
    info.currentSkills[1] = -1;
    info.activeAmmo[0] = 0;
    info.activeAmmo[1] = 0;
    info.currentWeapon = 0;
    info.enemiesRemaining = 0;
    info.hp = 3;
    info.inactiveAmmo[0] = 0;
    info.inactiveAmmo[1] = 0;
    info.score = 0;

    skillChoosen = false;
    constructGUIElements();
}

HUDManager::~HUDManager()
{
    //delete info;
}

void HUDManager::update(Player const &player, WaveTimeManager const &timeManager,
    EntityManager const &entityManager)
{
    //updates hudInfo with the current info
    info.score = ComboMachine::Get().GetCurrentScore();
    info.hp = player.getHP();
    info.activeAmmo[HUDManager::CURRENT_AMMO] =     player.getMainHand()->getMagAmmo();
    info.activeAmmo[HUDManager::TOTAL_AMMO] =   player.getMainHand()->getAmmo();
    info.inactiveAmmo[HUDManager::CURRENT_AMMO] =   player.getOffHand()->getMagAmmo();
    info.inactiveAmmo[HUDManager::TOTAL_AMMO] = player.getOffHand()->getAmmo();
    info.sledge = player.isUsingMeleeWeapon();
    info.currentWeapon = player.getCurrentWeapon();

   

    //skill cooldowns are inverted for some reason 
    const Skill* secondary = player.getSkill(SkillManager::ID::SECONDARY);
    if (!secondary->getCanUse())
        info.cd[0] = secondary->getCooldown() / secondary->getCooldownMax();
    else
        info.cd[0] = 1.0f;

    const Skill* primary = player.getSkill(SkillManager::ID::PRIMARY);
    if (!primary->getCanUse())
        info.cd[1] = primary->getCooldown() / primary->getCooldownMax();
    else
        info.cd[1] = 1.0f;



    info.wave = timeManager.getCurrentWave() + 1;
    info.timeRemaining = (timeManager.getTimeRequired() - timeManager.getTimeCurrent()) * 0.001f;
    info.enemiesRemaining = (int)entityManager.getNrOfAliveEnemies();


     info.currentSkills[0] = player.getCurrentSkill0();
    info.currentSkills[1] = player.getCurrentSkill1();

    this->updateGUIElemets();
    
}

void HUDManager::render() const
{    
    //render icons 
    for (auto &sprite : HUDElements)
    {
        sprite.render();
    }

    //renders skill masks
    int i = 0;
    for (auto &sprite : skillMasks)
    {
        if (1.0f - info.cd[i] > FLT_EPSILON)
        {
            sprite.render();
        }
        i++;
    }

    //render hp bars   
    for (auto &bar : staticElements)
    {
        bar.render();
    }
    for (auto &bar : HPBar)
    {
        bar.render();
    }

}

void Logic::HUDManager::reset()
{
    info.cd[0] = 1.0f;
    info.cd[1] = 1.0f;
    info.currentSkills[0] = -1;
    info.currentSkills[1] = -1;
    info.activeAmmo[0] = 0;
    info.activeAmmo[1] = 0;
    info.currentWeapon = 0;
    info.enemiesRemaining = 0;
    info.hp = 3;
    info.inactiveAmmo[0] = 0;
    info.inactiveAmmo[1] = 0;
    info.score = 0;

    skillChoosen = false;
    HUDElements.clear();
    skillList.clear();
    HPBar.clear();
    constructGUIElements();
}
