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
    HUDElements.push_back(Sprite(Sprite::BOTTOM_LEFT, Sprite::BOTTOM_LEFT, 50, -50, 230, 80, Resources::Textures::HPBars, FloatRect({ 0.0f, 0.0f }, { 1.f, 0.5f })));
    HUDElements.push_back(Sprite(Sprite::BOTTOM_LEFT, Sprite::BOTTOM_LEFT, 50, -50, 230, 80, Resources::Textures::HPBars, FloatRect({ 0.0f, 0.5f }, { 1.f, 1.f })));
    

    //crossbow
    float x = 339.0f / 2048;
    float y = 67.0f / 720;
    float width = 285.0f / 2048;
    float height = 276.0f / 720;
    HUDElements.push_back(Sprite(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -280, -50, 75, 75, Resources::Textures::HUDIcons, FloatRect({ x, y }, {x + width, y + height })));
    
    


    //ice staff

    x = 37.0f / 2048;
    y = 63.0f / 720;
    width = 284.0f / 2048;
    height = 279.0f / 720;

    HUDElements.push_back(Sprite(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -200, -50, 75, 75, Resources::Textures::HUDIcons, FloatRect({ x, y }, { x + width, y + height })));

    //sledegehammer
    x = 639.0f / 2048;
    y = 67.0f / 720;
    width = 286.0f / 2048;
    height = 275.0f / 720;

    HUDElements.push_back(Sprite(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -120, -50, 75, 75, Resources::Textures::HUDIcons, FloatRect({ x, y }, { x + width, y + height })));



    //creates the difrent skill icons 


    //skill 1
    GUIElements[6].alpha = 1;
    GUIElements[6].texture = Resources::Textures::HUDIcons;
    GUIElements[6].screenRect = FloatRect(1060.0f / WIN_WIDTH, 530.0f / WIN_HEIGHT, 178.0f / WIN_WIDTH, 100.0f / WIN_HEIGHT);
    GUIElements[6].textureRect = FloatRect(1000.0f / WIN_WIDTH, 370.0f / WIN_HEIGHT, 309.0f / WIN_WIDTH, 309.0f / WIN_HEIGHT);

    //skill 2
    GUIElements[7].alpha = 1;
    GUIElements[7].texture = Resources::Textures::HUDIcons;
    GUIElements[7].screenRect = FloatRect(1170 / WIN_WIDTH, 530 / WIN_HEIGHT, 178.0f / WIN_WIDTH, 100.f / WIN_HEIGHT);
    GUIElements[7].textureRect = FloatRect(1354 / WIN_WIDTH, 369 / WIN_HEIGHT, 307 / WIN_WIDTH, 307 / WIN_HEIGHT);
    
    //skill 3
    GUIElements[8].alpha = 1;
    GUIElements[8].texture = Resources::Textures::HUDIcons;
    GUIElements[8].screenRect = FloatRect(1170.0f / WIN_WIDTH, 530.0f / WIN_HEIGHT, 178.0f / WIN_WIDTH, 100.0f / WIN_HEIGHT);
    GUIElements[8].textureRect = FloatRect(1697.0f / WIN_WIDTH, 374.0f / WIN_HEIGHT, 306.0f / WIN_WIDTH, 306.0f / WIN_HEIGHT);

    //skill 1 cd
    GUIElements[9].alpha = 1;
    GUIElements[9].texture = Resources::Textures::HUDIcons;
    GUIElements[9].screenRect = FloatRect(1060.0f / WIN_WIDTH, 530.0f / WIN_HEIGHT, 178.0f / WIN_WIDTH, 100.0f / WIN_HEIGHT);
    GUIElements[9].textureRect = FloatRect(999.0f / WIN_WIDTH, 46.0f / WIN_HEIGHT, 300.0f / WIN_WIDTH, 300.0f / WIN_HEIGHT);

    //skill 2 cd
    GUIElements[10].alpha = 1;
    GUIElements[10].texture = Resources::Textures::HUDIcons;
    GUIElements[10].screenRect = FloatRect(1060.0f / WIN_WIDTH, 530.0f / WIN_HEIGHT, 178.0f / WIN_WIDTH, 100.0f / WIN_HEIGHT);
    GUIElements[10].textureRect = FloatRect(1351.0f / WIN_WIDTH, 43.0f / WIN_HEIGHT, 302.0f / WIN_WIDTH, 302.0f / WIN_HEIGHT);


    //skill 3 cd
    GUIElements[11].alpha = 1;
    GUIElements[11].texture = Resources::Textures::HUDIcons;
    GUIElements[11].screenRect = FloatRect(1060.0f / WIN_WIDTH, 530.0f / WIN_HEIGHT, 178.0f / WIN_WIDTH, 100.0f / WIN_HEIGHT);
    GUIElements[11].textureRect = FloatRect(1695.0f / WIN_WIDTH, 46.0f / WIN_HEIGHT, 303.0f / WIN_WIDTH, 303.0f / WIN_HEIGHT);


    


}

//updates the active weapons and cd icons
void Logic::HUDManager::updateGUIElemets()
{
    //hp
    if (prevHP != info.hp)
    {
        static float totalLenghtScreen = abs(HUDElements.at(HP_BARFRONT).getScreenRect().bottomRight.x - HUDElements.at(HP_BARFRONT).getScreenRect().topLeft.x);
        static float totalLenghtTexture = abs(HUDElements.at(HP_BARFRONT).getTextureRect().bottomRight.x - HUDElements.at(HP_BARFRONT).getTextureRect().topLeft.x);
        HUDElements.at(HP_BARFRONT).moveScreenPos(
            0.0f,
            0.0f,
            -(totalLenghtScreen *  (1.0f / (float)PLAYER_STARTING_HP)),
            0.0f
        );

        HUDElements.at(HP_BARFRONT).moveTexturePos(
            0.0f,
            0.0f,
            -(totalLenghtTexture * (1.0f / (float)PLAYER_STARTING_HP)),
            0.0f
        );
        prevHP = info.hp;
    }
    

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
    if (skillChoosen == false)
    {
        
        ////skill 1 off cd
        //choosenSKills[0].alpha = 1;
        //choosenSKills[0].texture = Resources::Textures::HUDIcons;
        //choosenSKills[0].screenRect = FloatRect(1060.0f / WIN_WIDTH, 530.0f / WIN_HEIGHT, 178.0f / WIN_WIDTH, 100.0f / WIN_HEIGHT);
        //choosenSKills[0].textureRect = GUIElements[6 + info.currentSkills[0]].textureRect;

        //// skill 1 on cd
        //choosenSKills[1].alpha = 1;
        //choosenSKills[1].texture = Resources::Textures::HUDIcons;
        //choosenSKills[1].screenRect = FloatRect(1170.0f / WIN_WIDTH, 530.0f / WIN_HEIGHT, 178.0f / WIN_WIDTH, 100.0f / WIN_HEIGHT);
        //choosenSKills[1].textureRect = GUIElements[6 + info.currentSkills[1]].textureRect;

        ////skill 2 off cd
        //choosenSKills[2].alpha = 1;
        //choosenSKills[2].texture = Resources::Textures::HUDIcons;
        //choosenSKills[2].screenRect = FloatRect(1060.0f / WIN_WIDTH, 530.0f / WIN_HEIGHT, 178.0f / WIN_WIDTH, 100.0f / WIN_HEIGHT);
        //choosenSKills[2].textureRect = GUIElements[9 + info.currentSkills[0]].textureRect;

        ////skill 2 on cd
        //choosenSKills[3].alpha = 1;
        //choosenSKills[3].texture = Resources::Textures::HUDIcons;
        //choosenSKills[3].screenRect = FloatRect(1170.0f / WIN_WIDTH, 530.0f / WIN_HEIGHT, 178.0f / WIN_WIDTH, 100.0f / WIN_HEIGHT);
        //choosenSKills[3].textureRect = GUIElements[9 + info.currentSkills[1]].textureRect;


        //skillChoosen = true;
    }
    else
    {
        /*if (1.0f - info.cd[0] < FLT_EPSILON)
        {
            GUIElements[6] = choosenSKills[0];
        }
        else
        {
            GUIElements[6] = choosenSKills[1];
        }


        if (1.0f - info.cd[1] < FLT_EPSILON)
        {
            GUIElements[7] = choosenSKills[2];
        }
        else
        {
            GUIElements[7] = choosenSKills[3];
        }*/
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
    prevHP = 3;

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

    // HUD info on the first skill
    const Skill* primary = player.getSkill(SkillManager::ID::PRIMARY);
    if (!primary->getCanUse())
        info.cd[0] = primary->getCooldown() / primary->getCooldownMax();
    else
        info.cd[0] = 1.0f;

    const Skill* secondary = player.getSkill(SkillManager::ID::SECONDARY);
    if (!secondary->getCanUse())
        info.cd[1] = secondary->getCooldown() / secondary->getCooldownMax();
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
    for (auto &sprite : HUDElements)
    {
        sprite.render();
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
    constructGUIElements();
}
