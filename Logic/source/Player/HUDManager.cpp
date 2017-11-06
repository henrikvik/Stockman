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
    HUDElements.push_back(Sprite( Sprite::CENTER, Sprite::CENTER, 0, 0, crosshairSize, crosshairSize, Resources::Textures::crosshair, FloatRect({ 0.0f, 0.0f }, { 1.f, 1.f })));

    //hpbars
   /* HUDElements.push_back(Sprite(Sprite::BOTTOM_LEFT, Sprite::BOTTOM_LEFT, 50, -50, 230, 80, Resources::Textures::HPBars, FloatRect({ 0.0f, 0.0f }, { 1.f, 0.5f })));
    HUDElements.push_back(Sprite(Sprite::BOTTOM_LEFT, Sprite::BOTTOM_LEFT, 50, -50, 230, 80, Resources::Textures::HPBars, FloatRect({ 0.0f, 0.5f }, { 1.f, 1.f })));*/
    

    //crossbow
    float x = 339.0f / 2048;
    float y = 67.0f / 720;
    float width = 285.0f / 2048;
    float height = 276.0f / 720;
    HUDElements.push_back(Sprite(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -320, -50, 75, 75, Resources::Textures::HUDIcons, FloatRect({ x, y }, {x + width, y + height })));
    
    


    //ice staff

    x = 37.0f / 2048;
    y = 63.0f / 720;
    width = 284.0f / 2048;
    height = 279.0f / 720;

    HUDElements.push_back(Sprite(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -240, -50, 75, 75, Resources::Textures::HUDIcons, FloatRect({ x, y }, { x + width, y + height })));

    //sledegehammer
    x = 639.0f / 2048;
    y = 67.0f / 720;
    width = 286.0f / 2048;
    height = 275.0f / 720;

    HUDElements.push_back(Sprite(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -160, -50, 75, 75, Resources::Textures::HUDIcons, FloatRect({ x, y }, { x + width, y + height })));



    //creates the difrent skill icons 

    //grappling inactive
    x = 999.0f / 2048;
    y = 46.0f / 720;
    width = 300.0f / 2048;
    height = 300.0f / 720;
    skillList.push_back(Sprite(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -80, -50, 75, 75, Resources::Textures::HUDIcons, FloatRect({ x, y }, { x + width, y + height })));

    //charge inacitve
    x = 1351.f / 2048;
    y = 43.0f / 720;
    width = 302.0f / 2048;
    height = 302.0f / 720;
    skillList.push_back(Sprite(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -80, -50, 75, 75, Resources::Textures::HUDIcons, FloatRect({ x, y }, { x + width, y + height })));


    //bullet time inactive
    x = 1695.f / 2048;
    y = 46.0f / 720;
    width = 303.0f / 2048;
    height = 303.0f / 720;
    skillList.push_back(Sprite(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -80, -50, 75, 75, Resources::Textures::HUDIcons, FloatRect({ x, y }, { x + width, y + height })));


    //grappling active
    x = 1000.0f / 2048;
    y = 370.0f / 720;
    width = 309.0f / 2048;
    height = 309.0f / 720;
    skillList.push_back(Sprite(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -80, -50, 75, 75, Resources::Textures::HUDIcons, FloatRect({ x, y }, { x + width, y + height })));

    //charge active
    x = 1354.f / 2048;
    y = 369.0f / 720;
    width = 307.0f / 2048;
    height = 307.0f / 720;
    skillList.push_back(Sprite(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -80, -50, 75, 75, Resources::Textures::HUDIcons, FloatRect({ x, y }, { x + width, y + height })));


    //bullet time active
    x = 1697.f / 2048;
    y = 374.0f / 720;
    width = 306.0f / 2048;
    height = 306.0f / 720;
    skillList.push_back(Sprite(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -80, -50, 75, 75, Resources::Textures::HUDIcons, FloatRect({ x, y }, { x + width, y + height })));
 
    for (size_t i = 0; i < PLAYER_STARTING_HP; i++)
    {
        HPBar.push_back(Sprite(Sprite::BOTTOM_LEFT, Sprite::BOTTOM_LEFT, 50 + (i * 75), -50, 75, 75, Resources::Textures::HPBars, FloatRect({ 0.0f, 0.0f }, { 1.f, 1.f })));
    }
}

//updates the active weapons and cd icons
void Logic::HUDManager::updateGUIElemets()
{
    //hp

    if (HPBar.size() != info.hp && info.hp != 0)
    {
        HPBar.pop_back();
    }
    /*HPBar.clear();
    for (size_t i = 0; i < info.hp; i++)
    {
        HPBar.push_back(Sprite(Sprite::BOTTOM_LEFT, Sprite::BOTTOM_LEFT, 50 + (i *75), -50, 75, 75, Resources::Textures::HPBars, FloatRect({ 0.0f, 0.0f }, { 1.f, 1.f })));
    }*/
    

    //crossbow
    if (info.currentWeapon == 0)
    {
        HUDElements.at(CROSSBOW).setTexturePos(334.0f / 2048.f, 385.0f / 720.f, 620.0f / 2048.f, 672.0f / 720.f);
    }
    else {
        
        HUDElements.at(CROSSBOW).setTexturePos(339.0f / 2048.f, 66.0f / 720.f, 624.0f / 2048.f, 342.0f / 720.f);
    }
    //ice staff
    if (info.currentWeapon == 1)
    {

        HUDElements.at(ICESTAFF).setTexturePos(29.0f / 2048.f, 384.0f / 720.f, 319.0f / 2048.f, 673.0f / 720.f);
    }
    else
    {
        HUDElements.at(ICESTAFF).setTexturePos(37.0f / 2048.f, 63.0f / 720.f, 320.0f / 2048.f, 341.0f / 720.f);
    }

    //sledgehammer
    if (info.currentWeapon == 2)
    {
        HUDElements.at(SLEDGEHAMMER).setTexturePos(635.0f / 2048.f, 387.0f / 720.f, 920.0f / 2048.f, 672.0f / 720.f);
    }
    else
    {
        HUDElements.at(SLEDGEHAMMER).setTexturePos(639.0f / 2048.f, 67.0f / 720.f, 924.0f / 2048.f, 341.0f / 720.f);
    }


    //skills
    if (skillChoosen == false && info.currentSkills[0] >= 0 && info.currentSkills[1] >= 0)
    {
        
        HUDElements.push_back(skillList.at(info.currentSkills[0]));

        skillList.at(info.currentSkills[1]).moveScreenPos(80.0f, 0.0f);
        skillList.at(3 + info.currentSkills[1]).moveScreenPos(80.0f, 0.0f);

        HUDElements.push_back(skillList.at(info.currentSkills[1]));


        skillChoosen = true;
    }
    else
    {
        if (1.0f - info.cd[0] < FLT_EPSILON && info.currentSkills[0] >= 0)
        {
            HUDElements.at(SKILL1) = skillList.at(3 + info.currentSkills[0]);
        }
        else if (info.currentSkills[0] >= 0)
        {
            HUDElements.at(SKILL1) = skillList.at(info.currentSkills[0]);
        }


        if (1.0f - info.cd[1] < FLT_EPSILON && info.currentSkills[1] >= 0)
        {
            HUDElements.at(SKILL2) = skillList.at(3 + info.currentSkills[1]);
        }
        else if (info.currentSkills[1] >= 0)
        {
            HUDElements.at(SKILL2) = skillList.at(info.currentSkills[1]);
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

    //render hp bars
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
