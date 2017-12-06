#include <Player\HUDManager.h>
#include <Player\Player.h> 
#include <Player\Skill\Skill.h>
#include <Player\Skill\SkillManager.h>
#include <Player\Weapon\Weapon.h>
#include <Player\Weapon\AmmoContainer.h>

#include <AI\WaveTimeManager.h>
#include <AI\EntityManager.h>

#include <Misc\ComboMachine.h>

#include <comdef.h>

using namespace Logic;

const int HUDManager::CURRENT_AMMO = 0;
const int HUDManager::TOTAL_AMMO = 1;

const std::wstring HUDManager::IN_WAVE     = L"MURDER",
                   HUDManager::BEFORE_WAVE = L"PREPARE",
                   HUDManager::AFTER_WAVE  = L"ENRAGED";

const float HUDManager::WAVE_SLIDE_TIME = 5000.0f;
const float HUDManager::ENRAGE_SLIDE_TIME = 3000.0f;
const float HUDManager::PICKEUP_MESSAGE_TIMER = 2000.0f;

HUDManager::HUDManager()
{
//    ZeroMemory(&info, sizeof(info)); // Yes, this causes a "fake" memory leaks
    info.cd[0] = 1.0f;
    info.cd[1] = 1.0f;
    info.cdInSeconds[0] = 0;
    info.cdInSeconds[1] = 0;
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
    info.scoreCombo = 0;
    info.scoreMul = 0;
    crossBowTimer = -1.0f;
    staffTimer = -1.0f;

    skillChoosen = false;
    constructGUIElements();
    showWaveCleared = false;
    nextWaveSlideTimer = WAVE_SLIDE_TIME;
    enrageSlideTimer = ENRAGE_SLIDE_TIME;
    wasEnraged = false;

    crossBowTimer = -1.0f;
    staffTimer = -1.0f;
}

HUDManager::~HUDManager()
{
}

//constructs the spriteRenderInfos that the hud consists of
void HUDManager::constructGUIElements()
{
    //crosshair
    float crosshairSize = 50;
    HUDElements.push_back(Sprite( Sprite::CENTER, Sprite::CENTER, 0, 0, crosshairSize, crosshairSize, Resources::Textures::Crosshair, FloatRect({ 0.0f, 0.0f }, { 1.f, 1.f })));

    //crossbow
    float x = 3.0f / 1024;
    float y = 517.0f / 1024;
    float width = 503.0f / 1024;
    float height = 503.0f / 1024;
    HUDElements.push_back(Sprite(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -65, -175, 110, 110, Resources::Textures::weaponsheet, FloatRect({ x, y }, {x + width, y + height }), 1.0f, true));
   
    //ice staff
    x = 4.0f / 1024;
    y = 5.0f / 1024;
    width = 503.0f / 1024;
    height = 503.0f / 1024;

    HUDElements.push_back(Sprite(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -65, -60, 110, 110, Resources::Textures::weaponsheet, FloatRect({ x, y }, { x + width, y + height }), 1.0f, true));
    
    //sledegehammer
    x = 519.0f / 1024;
    y = 9.0f / 1024;
    width = 503.0f / 1024;
    height = 503.f / 1024;

    HUDElements.push_back(Sprite(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -132, -25, 75, 75, Resources::Textures::weaponsheet, FloatRect({ x, y }, { x + width, y + height }), 1.0f, true));

    //creates the weapon mask
    x = 518.0f / 1024;
    y = 519.0f / 1024;
    width = 503.0f / 1024;
    height = 503.f / 1024;
    HUDElements.push_back(Sprite(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -65, -60, 110, 110, Resources::Textures::weaponsheet, FloatRect({ x, y }, { x + width, y + height }), 1.0f, true));

    //creates the difrent skill icons 

    //grappling 
    x = 519.0f / 1024;
    y = 9.0f / 1024;
    width = 503.0f / 1024;
    height = 503.f / 1024;
    skillList.push_back(Sprite(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -60, -148, 50, 50, Resources::Textures::Spellsheet, FloatRect({ x, y }, { x + width, y + height }), 1.0f, true));

    //charge 
    x = 3.0f / 1024;
    y = 517.0f / 1024;
    width = 503.0f / 1024; 
    height = 503.0f / 1024;
    skillList.push_back(Sprite(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -60, -148, 50, 50, Resources::Textures::Spellsheet, FloatRect({ x, y }, { x + width, y + height }), 1.0f, true));
    

    //bullet time 
    x = 4.0f / 1024;
    y = 5.0f / 1024;
    width = 503.0f / 1024;
    height = 503.0f / 1024;
    skillList.push_back(Sprite(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -60, -148, 50, 50, Resources::Textures::Spellsheet, FloatRect({ x, y }, { x + width, y + height }), 1.0f, true));

    //skill mask 1
    x = 518.0f / 1024;
    y = 519.0f / 1024;
    width = 503.0f / 1024;
    height = 503.f / 1024;
    skillMasks.push_back(Sprite(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -60, -148, 50, 50, Resources::Textures::Spellsheet, FloatRect({ x, y }, { x + width, y + height }), 1.0f, true));

    //skill mask 2
    x = 518.0f / 1024;
    y = 519.0f / 1024;
    width = 503.0f / 1024;
    height = 503.f / 1024;
    skillMasks.push_back(Sprite(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -130, -148, 50, 50, Resources::Textures::Spellsheet, FloatRect({ x, y }, { x + width, y + height }), 1.0f, true));

    for (size_t i = 0; i < PLAYER_STARTING_HP; i++)
    {
        x = 20.0 / 1024;
        y = 450.0f / 1024;
        width = 170.0f / 1024;
        height = 180.0f / 1024;
        HPBar.push_back(Sprite(Sprite::BOTTOM_LEFT, Sprite::BOTTOM_LEFT, 85 + (i * 25), -55, 35, 40, Resources::Textures::Gamesheet, FloatRect({ x, y }, {x + width, y + height }), 1.0f, true));

        x = 260.0f / 1024;
        y = 450.0f / 1024;
        width = 170.0f / 1024;
        height = 180.0f / 1024;
        staticElements.push_back(Sprite(Sprite::BOTTOM_LEFT, Sprite::BOTTOM_LEFT, 85 + (i * 25), -55, 35, 40, Resources::Textures::Gamesheet, FloatRect({ x, y }, { x + width, y + height }), 1.0f, true));
    }
    //static hud elemets

    // hp icon
    x = 505.0f / 1024;
    y = 491.0f / 1024;
    width = 106.0f / 1024;
    height = 106.0f / 1024;
    staticElements.push_back(Sprite(Sprite::BOTTOM_LEFT, Sprite::BOTTOM_LEFT, 45, -60, 30, 30, Resources::Textures::Gamesheet, FloatRect({ x, y }, { x + width, y + height }), 1.0f, true));

    //score banner
    x = 26.f / 1024;
    y = 23.0f / 1024;
    width = 364.0f / 1024;
    height = 90.0f / 1024;
    staticElements.push_back(Sprite(Sprite::TOP_LEFT, Sprite::TOP_LEFT, 60, 25, 180, 40, Resources::Textures::Gamesheet, FloatRect({ x, y }, { x + width, y + height })));


    //wave counter
    x = 30.0f / 1024;
    y = 140.0f / 1024;
    width = 575.0f / 1024;
    height = 90.0f / 1024;
    staticElements.push_back(Sprite(Sprite::TOP_LEFT, Sprite::TOP_LEFT, 505, 25, 340, 40, Resources::Textures::Gamesheet, FloatRect({ x, y }, { x + width, y + height })));

    //hotkey skill 1
    x = 372.f / 1024;
    y = 266.0f / 1024;
    width = 129.0f / 1024;
    height = 148.0f / 1024;
    staticElements.push_back(Sprite(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -170, -136, 20, 20, Resources::Textures::Gamesheet, FloatRect({ x, y }, { x + width, y + height }), 1.0f, true));

    //hotkey skill 2
    x = 595.f / 1024;
    y = 266.0f / 1024;
    width = 129.0f / 1024;
    height = 148.0f / 1024;
    staticElements.push_back(Sprite(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -50, -136, 20, 20, Resources::Textures::Gamesheet, FloatRect({ x, y }, { x + width, y + height }), 1.0f, true));

    //remaining enemies
    x = 26.f / 1024;
    y = 23.0f / 1024;
    width = 364.0f / 1024;
    height = 90.0f / 1024;
    staticElements.push_back(Sprite(Sprite::TOP_LEFT, Sprite::TOP_LEFT, 1050, 25, 180, 40, Resources::Textures::Gamesheet, FloatRect({ x, y }, { x + width, y + height })));

    //next wave
    waveSprites.push_back(Sprite(Sprite::CENTER, Sprite::CENTER, 0, -200, 450, 200, Resources::Textures::wavebegins, FloatRect({ 0.0f, 0.0f }, { 1.0f, 1.0f }), 0.0f, false));

    //enrage
    waveSprites.push_back(Sprite(Sprite::CENTER, Sprite::CENTER, 0, -200, 450, 200, Resources::Textures::Enraged, FloatRect({ 0.0f, 0.0f }, { 1.0f, 1.0f }), 0.0f, false));
}

void HUDManager::updateTextElements()
{
    HUDText.clear();

    //cd 1 text
    TextRenderInfo text;
    text.color = DirectX::SimpleMath::Color(1, 1, 1, 1);
    if (info.cdInSeconds[0] > 0)
    {
        text.text = std::to_wstring(info.cdInSeconds[0]);
        text.position = DirectX::SimpleMath::Vector2(1185, 530);
        text.font = Resources::Fonts::KG14;

        text.isMoveable = true;

        HUDText.push_back(TextRenderInfo(text));
    }
    
    //cd 2 text
    if (info.cdInSeconds[1] > 0)
    {
        text.text = std::to_wstring(info.cdInSeconds[1]);
        text.position = DirectX::SimpleMath::Vector2(1115, 530);
        text.font = Resources::Fonts::KG18;
        text.isMoveable = true;
        HUDText.push_back(TextRenderInfo(text));
    }
    
    //points
    text.text = L"Score: " + std::to_wstring(info.score);
    text.position = DirectX::SimpleMath::Vector2(80, 35);
    text.font = Resources::Fonts::KG14;
    text.isMoveable = false;
    HUDText.push_back(TextRenderInfo(text));

    text.isMoveable = false;

    text.text = std::to_wstring(info.scoreCombo);
    text.position = DirectX::SimpleMath::Vector2(230, 35);
    text.font = Resources::Fonts::KG14;

    HUDText.push_back(TextRenderInfo(text));


    text.text = (std::to_wstring(info.scoreMul) + L"X");
    text.position = DirectX::SimpleMath::Vector2(205, 35);
    text.font = Resources::Fonts::KG14;

    HUDText.push_back(TextRenderInfo(text));

    //total ammo of weapon
    if (info.currentWeapon == 0)
    {
        if (info.activeAmmo[1] > 0)
        {
            text.text = std::to_wstring(/*info.activeAmmo[0] +*/ info.activeAmmo[1]);
            text.position = DirectX::SimpleMath::Vector2(1183, 430);
            text.font = Resources::Fonts::KG14;
            text.isMoveable = true;

            HUDText.push_back(TextRenderInfo(text));
        }
        
        if (info.inactiveAmmo[1] > 0)
        {
            text.text = std::to_wstring(/*info.activeAmmo[0] +*/ info.inactiveAmmo[1]);
            text.position = DirectX::SimpleMath::Vector2(1183, 630);
            text.font = Resources::Fonts::KG14;
            text.isMoveable = true;
            HUDText.push_back(TextRenderInfo(text));
        }
      
    }
    else
    {
        if (info.activeAmmo[1] > 0)
        {
            text.text = std::to_wstring(/*info.activeAmmo[0] +*/ info.activeAmmo[1]);
            text.position = DirectX::SimpleMath::Vector2(1183, 632);
            text.isMoveable = true;
            text.font = Resources::Fonts::KG14;
            HUDText.push_back(TextRenderInfo(text));
        }

        if (info.inactiveAmmo[1] > 0)
        {
            text.text = std::to_wstring(/*info.activeAmmo[0] +*/ info.inactiveAmmo[1]);
            text.position = DirectX::SimpleMath::Vector2(1183, 428);
            text.font = Resources::Fonts::KG14;
            text.isMoveable = true;
            HUDText.push_back(TextRenderInfo(text));
        }

    }

    //current ammo in mag of active weapon
    if (info.currentWeapon != 2)
    {
        if (info.isReloding)
        {
            text.text = L"RELOADING";
            text.position = DirectX::SimpleMath::Vector2(680, 380);
            text.font = Resources::Fonts::KG14;

            text.isMoveable = false;
            HUDText.push_back(TextRenderInfo(text));
        }
        else
        {
            if (info.activeAmmo[1] > 0)
            {
                text.color = DirectX::SimpleMath::Color(0.545f, 0.000f, 0.000f);
            }
            else
            {
                text.color = DirectX::SimpleMath::Color(1, 1, 1, 1);
            }

            text.text = std::to_wstring(info.activeAmmo[0]);
            text.position = DirectX::SimpleMath::Vector2(680, 380);
            text.font = Resources::Fonts::KG14;

            text.isMoveable = false;
            HUDText.push_back(TextRenderInfo(text));
        }
        
    }

    //time and enrage/ survive
    int minutes = info.timeRemaining / 60;
    int seconds = info.timeRemaining - (minutes*60);
    std::wstring timeString = L"";
    text.isMoveable = false;
    text.color = DirectX::SimpleMath::Color(1, 1, 1, 1);
    if (seconds < 0)
    {
        timeString = L"0:00";
        text.text = timeString;
        text.position = DirectX::SimpleMath::Vector2(790, 30);
        text.font = Resources::Fonts::KG14;

        HUDText.push_back(TextRenderInfo(text));

        text.color = DirectX::SimpleMath::Color(0.9f, 0.9f, 0.9f);
        text.text = info.waveText;
        text.position = DirectX::SimpleMath::Vector2(550, 30);
        text.font = Resources::Fonts::KG14;

        HUDText.push_back(TextRenderInfo(text));
    }
    else
    {
        if (seconds < 10)
        {
            timeString = std::to_wstring(minutes) + L":0" + std::to_wstring(seconds);
        }
        else
        {
            timeString = std::to_wstring(minutes) + L":" + std::to_wstring(seconds);
        }
        text.text = timeString;
        text.position = DirectX::SimpleMath::Vector2(790, 30);
        text.font = Resources::Fonts::KG14;

        HUDText.push_back(TextRenderInfo(text));

        text.color = DirectX::SimpleMath::Color(0.9f, 0.9f, 0.9f);
        text.text = info.waveText;
        text.position = DirectX::SimpleMath::Vector2(550, 30);
        text.font = Resources::Fonts::KG14;

        HUDText.push_back(TextRenderInfo(text));
    }
    
    //wave counter
    text.color = DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f);
    text.text = L"Wave: " + std::to_wstring(info.wave) + L"/" + std::to_wstring(info.maxWaves);
    text.position = DirectX::SimpleMath::Vector2(680, 30);
    text.font = Resources::Fonts::KG14;

    HUDText.push_back(TextRenderInfo(text));


    //next wave text
    if (waveSprites.size() && 0.5f - waveSprites.at(WaveMessages::NEXTWAVE).getAlpha() < FLT_EPSILON)
    {
        text.color = DirectX::SimpleMath::Color(0.38671875f, 0.1796875f, 0.125f);
        text.font = Resources::Fonts::nordic25;
        text.position = DirectX::SimpleMath::Vector2(WIN_WIDTH / 2.0f - 24, WIN_HEIGHT / 2.0f - 222);
        text.text = std::to_wstring(info.wave);

        HUDText.push_back(TextRenderInfo(text));
    }


    //enmeies remaining text
    if (info.enemiesRemaining > 0)
    {
        text.color = DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f);
        text.font = Resources::Fonts::KG14;
        text.position = DirectX::SimpleMath::Vector2(1060, 35);
        text.text = L"REMAINING: " + std::to_wstring(info.enemiesRemaining);

        HUDText.push_back(TextRenderInfo(text));
    }

    //pickup message
    if (crossBowTimer > 0.0f)
    {
        text.color = DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f);
        text.font = Resources::Fonts::KG14;
        text.position = DirectX::SimpleMath::Vector2(515, 470);
        text.text = L"ENHANCED CROSSBOW AMMO PICKED UP";
        HUDText.push_back(TextRenderInfo(text));
    }
   
    if (staffTimer > 0.0f)
    {
        text.color = DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f);
        text.font = Resources::Fonts::KG14;
        text.position = DirectX::SimpleMath::Vector2(515, 490);
        text.text = L"ENHANCED STAFF AMMO PICKED UP";
        HUDText.push_back(TextRenderInfo(text));
    }

}

//updates the active weapons and cd icons
void HUDManager::updateGUIElemets()
{
    //hp
    if (!HPBar.empty() && HPBar.size() > info.hp)
    {
        HPBar.pop_back();
    }

    if (HPBar.size() < info.hp)
    {
		float x = 20.0 / 1024;
		float y = 450.0f / 1024;
		float width = 170.0f / 1024;
		float height = 180.0f / 1024;
		HPBar.push_back(Sprite(Sprite::BOTTOM_LEFT, Sprite::BOTTOM_LEFT, 85 + (HPBar.size() * 25), -55, 35, 40, Resources::Textures::Gamesheet, FloatRect({ x, y }, { x + width, y + height }), 1.0f, true));
	}

    //decideds active weapon 
    switch (info.currentWeapon)
    {
    case 0:
        //crossbow
        HUDElements.at(WEAPONMASK).setScreenPos(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -65, -175, 110, 110);
        break;
        
    case 1:
        //icestaff
        HUDElements.at(WEAPONMASK).setScreenPos(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -65, -60, 110, 110);
        break;
        
    case 2:
        //sledgehammer
        HUDElements.at(WEAPONMASK).setScreenPos(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -132, -25, 75, 75);
        break;
    default:
        break;
    }

    //skills
    if (skillChoosen == false && info.currentSkills[0] >= 0 && info.currentSkills[1] >= 0 && info.currentSkills[0] <= 2 && info.currentSkills[1] <= 2)
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
            //skillMasks.at(0).setTopYToProcent(1.0f - info.cd[0], 519.0f / 1024, 0.725000024f, 0.794444442f);
        }
        else
        {
            skillMasks.at(0).setScreenPos(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -60, -148, 50, 50);
            float x = 518.0f / 1024;
            float y = 519.0f / 1024;
            float width = 503.0f / 1024;
            float height = 503.f / 1024;
            skillMasks.at(0).setTexturePos(x, y, x + width, y + height);
        }
        
        if (info.currentSkills[1] >= 0 && 1.0f - info.cd[1] > FLT_EPSILON)
        {
            //skillMasks.at(1).setTopYToProcent(1.0f , 519.0f / 1024, 0.725000024f, 0.794444442f);
        }
        else
        {
            skillMasks.at(1).setScreenPos(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -130, -148, 50, 50);
            float x = 518.0f / 1024;
            float y = 519.0f / 1024;
            float width = 503.0f / 1024;
            float height = 503.f / 1024;
            skillMasks.at(1).setTexturePos(x, y, x + width, y + height);
        }
    }

    if (info.enemiesRemaining == 0)
    {
        staticElements.at(staticElements.size() - 1).setAlpha(0.0f);
    }
    else
    {
        staticElements.at(staticElements.size() - 1).setAlpha(1.0f);
    }

   
}

void HUDManager::renderTextElements() const
{
    for (auto &text : HUDText)
    {
        QueueRender(text);
    }
}

void HUDManager::update(Player &player, WaveTimeManager const &timeManager,
    EntityManager const &entityManager, float dt)
{
    //updates hudInfo with the current info
    info.score = ComboMachine::Get().getTotalScore();
    info.scoreCombo = ComboMachine::Get().getComboScore();
    info.scoreMul = ComboMachine::Get().getCurrentCombo();
    info.hp = player.getHP();
    info.activeAmmo[HUDManager::CURRENT_AMMO]   = player.getActiveAmmoContainer().getAmmoInfo().magAmmo;// TODO GET AMMO
    info.activeAmmo[HUDManager::TOTAL_AMMO]     = player.getActiveAmmoContainer().getAmmoInfo().enhancedAmmo;// TODO GET AMMO
    info.inactiveAmmo[HUDManager::CURRENT_AMMO] = player.getInactiveAmmoContainer().getAmmoInfo().magAmmo;// TODO GET AMMO
    info.inactiveAmmo[HUDManager::TOTAL_AMMO]   = player.getInactiveAmmoContainer().getAmmoInfo().enhancedAmmo;// TODO GET AMMO
    info.sledge = player.isUsingMeleeWeapon();
    info.currentWeapon = player.getCurrentWeapon();
    info.isReloding = player.getReloding();
    info.ammoPickedUp = player.getAmmoPickedUp();


    //skill cooldowns are inverted for some reason 
    const Skill* secondary = player.getSkill(SkillManager::ID::SECONDARY);

    // skill is active
    if (secondary->isActive())
    {
        info.cd[0] = 0.f;
        info.cdInSeconds[0] = 0;
    }
    // skill is not active and on cooldown
    else if (!secondary->getCanUse()) 
    {
        info.cd[0] = secondary->getCooldown() / secondary->getCooldownMax();
        info.cdInSeconds[0] = secondary->getCooldown() / 1000 + 1.f;   
    }
    // skill is ready to use again
    else
    {
        info.cd[0] = 1.0f;
        info.cdInSeconds[0] = 0;
    }
    
    const Skill* primary = player.getSkill(SkillManager::ID::PRIMARY);
    // skill is active
    if (primary->isActive()) {
        info.cd[1] = 0.f;
        info.cdInSeconds[1] = 0;
    }
    // skill is not active and on cooldown
    else if (!primary->getCanUse()) {
        info.cd[1] = primary->getCooldown() / primary->getCooldownMax();
        info.cdInSeconds[1] = primary->getCooldown() / 1000 + 1.f;
    }
    // skill is ready to use again
    else {
        info.cd[1] = 1.0f;
        info.cdInSeconds[1] = 0;
    }
        
    info.wave = timeManager.getCurrentWave();
    info.maxWaves = entityManager.getWaveManager().getWaveInformation().nrOfWaves;
    if (timeManager.isEnraged())
    {
        info.waveText = AFTER_WAVE;
        if (wasEnraged == false)
        {
            enrageSlideTimer = ENRAGE_SLIDE_TIME;
            wasEnraged = true;
        }
        static float alpha1 = 0.0f;
        if (enrageSlideTimer > 0.0f)
        {
            enrageSlideTimer -= dt;

            if (enrageSlideTimer > ENRAGE_SLIDE_TIME / 2.0f)
            {
                if (alpha1 < 1.0f)
                {
                    alpha1 += dt *0.002f;
                }
                waveSprites.at(WaveMessages::ENRAGE).setAlpha(alpha1);
            }
            else
            {
                if (alpha1 > 0.0f)
                {
                    alpha1 -= dt *0.002f;
                }
                waveSprites.at(WaveMessages::ENRAGE).setAlpha(alpha1);
            }
        }
    }
    else if (timeManager.isTransitioning())
    {
        info.waveText = BEFORE_WAVE;
        nextWaveSlideTimer = WAVE_SLIDE_TIME;
        wasEnraged = false;
    }
    else {
        info.waveText = IN_WAVE;
        static float alpha = 0.0f;
        if (nextWaveSlideTimer > 0.0f)
        {
            nextWaveSlideTimer -= dt;

            if (nextWaveSlideTimer > WAVE_SLIDE_TIME / 2.0f)
            {
                if (alpha < 1.0f)
                {
                    alpha += dt *0.002f;
                }
                waveSprites.at(WaveMessages::NEXTWAVE).setAlpha(alpha);
            }
            else
            {
                if (alpha > 0.0f)
                {
                    alpha -= dt *0.002f;
                }
                waveSprites.at(WaveMessages::NEXTWAVE).setAlpha(alpha);
            }
        }
        wasEnraged = false;
    }

        

    info.timeRemaining = (timeManager.getTimeRequired() - timeManager.getTimeCurrent()) * 0.001f;
    info.enemiesRemaining = static_cast<int> (entityManager.getNrOfAliveEnemies());

    info.currentSkills[0] = player.getCurrentSkill0();
    info.currentSkills[1] = player.getCurrentSkill1();

    if (crossBowTimer > 0.0f)
        crossBowTimer -= dt;
 
    if (staffTimer > 0.0f)
        staffTimer -= dt;

    if (info.ammoPickedUp != 0 && info.ammoPickedUp < 3)
    {
        if (info.ammoPickedUp == 1)
        {
            crossBowTimer = PICKEUP_MESSAGE_TIMER;
        }
        else
        {
            staffTimer = PICKEUP_MESSAGE_TIMER;
        }
    }

    this->updateGUIElemets();
    this->updateTextElements();

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

    for (auto &wave : waveSprites)
    {
        wave.render();
    }
    renderTextElements();
}

void HUDManager::reset()
{
    info.cd[0] = 1.0f;
    info.cd[1] = 1.0f;
    info.cdInSeconds[0] = 0;
    info.cdInSeconds[1] = 0;
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
    info.scoreCombo = 0;
    info.scoreMul = 0;
    crossBowTimer = -1.0f;
    staffTimer = -1.0f;

    skillChoosen = false;
    showWaveCleared = false;
    wasEnraged = false;
    nextWaveSlideTimer = WAVE_SLIDE_TIME;
    enrageSlideTimer = ENRAGE_SLIDE_TIME;
    
    
    HUDElements.clear();
    skillList.clear();
    HPBar.clear();
    skillMasks.clear();
    staticElements.clear();
    HUDText.clear();
    waveSprites.clear();

    constructGUIElements();
   
}
