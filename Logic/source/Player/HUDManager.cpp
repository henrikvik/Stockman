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

const std::wstring HUDManager::IN_WAVE     = L"MURDER!",
                   HUDManager::BEFORE_WAVE = L"WAITING",
                   HUDManager::AFTER_WAVE  = L"ENRAGED!";

HUDManager::HUDManager()
{
//    ZeroMemory(&info, sizeof(info)); // Yes, this causes a "fake" memory leaks
    info.cd[0] = 1.0f;
    info.cd[1] = 1.0f;
    info.currentSkills[0] = -1;
    info.currentSkills[1] = -1;
    info.hp = 3;

    skillChoosen = false;
    constructGUIElements();
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
    HUDElements.push_back(Sprite(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -65, -175, 110, 110, Resources::Textures::weaponsheet, FloatRect({ x, y }, {x + width, y + height })));
   
    //ice staff
    x = 4.0f / 1024;
    y = 5.0f / 1024;
    width = 503.0f / 1024;
    height = 503.0f / 1024;

    HUDElements.push_back(Sprite(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -65, -60, 110, 110, Resources::Textures::weaponsheet, FloatRect({ x, y }, { x + width, y + height })));
    
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

    //hotkey skill 1
    x = 372.f / 1024;
    y = 266.0f / 1024;
    width = 129.0f / 1024;
    height = 148.0f / 1024;
    staticElements.push_back(Sprite(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -170, -136, 20, 20, Resources::Textures::Gamesheet, FloatRect({ x, y }, { x + width, y + height })));

    //hotkey skill 2
    x = 595.f / 1024;
    y = 266.0f / 1024;
    width = 129.0f / 1024;
    height = 148.0f / 1024;
    staticElements.push_back(Sprite(Sprite::BOTTOM_RIGHT, Sprite::BOTTOM_RIGHT, -50, -136, 20, 20, Resources::Textures::Gamesheet, FloatRect({ x, y }, { x + width, y + height })));

    //wave complete
    //x = 0.1;
    //y = 0.1;
    //width = 0.9;
    //height = 0.9;
    //staticElements.push_back(Sprite(Sprite::TOP_LEFT, Sprite::TOP_LEFT, 0, 0, 512.f, 128.0f, Resources::Textures::WaveComplete, FloatRect({ x, y }, { x + width, y + height })));
}

void HUDManager::updateTextElements()
{
    HUDText.clear();
    liveText.clear();
    int last = 0;

    //cd 1 text
    TextRenderInfo text;
    text.color = DirectX::SimpleMath::Color(1, 1, 1, 1);
    if (info.cdInSeconds[0] > 0)
    {
        liveText.push_back(std::to_wstring(info.cdInSeconds[0]));
        text.text = liveText.at(last).c_str();
        last++;
        text.position = DirectX::SimpleMath::Vector2(1185, 530);
        text.font = Resources::Fonts::KG18;

        HUDText.push_back(TextRenderInfo(text));
    }
    
    //cd 2 text
    if (info.cdInSeconds[1] > 0)
    {
        liveText.push_back(std::to_wstring(info.cdInSeconds[1]));
        text.text = liveText.at(last).c_str();
        last++;
        text.position = DirectX::SimpleMath::Vector2(1115, 530);
        text.font = Resources::Fonts::KG18;

        HUDText.push_back(TextRenderInfo(text));
    }
    
    //points
    liveText.push_back(std::to_wstring(info.score));
    text.text = liveText.at(last).c_str();
    last++;
    text.position = DirectX::SimpleMath::Vector2(142, 15);
    text.font = Resources::Fonts::KG14;

    HUDText.push_back(TextRenderInfo(text));

    //total ammo of weapon
    if (info.currentWeapon == 0)
    {
        liveText.push_back(std::to_wstring(/*info.activeAmmo[0] +*/ info.activeAmmo[1]));
        text.text = liveText.at(last).c_str();
        last++;
        text.position = DirectX::SimpleMath::Vector2(1183, 430);
        text.font = Resources::Fonts::KG14;

        HUDText.push_back(TextRenderInfo(text));

        liveText.push_back(std::to_wstring(/*info.activeAmmo[0] +*/ info.inactiveAmmo[1]));
        text.text = liveText.at(last).c_str();
        last++;
        text.position = DirectX::SimpleMath::Vector2(1183, 630);
        text.font = Resources::Fonts::KG14;

        HUDText.push_back(TextRenderInfo(text));
    }
    else
    {
        liveText.push_back(std::to_wstring(/*info.activeAmmo[0] +*/ info.activeAmmo[1]));
        text.text = liveText.at(last).c_str();
        last++;
        text.position = DirectX::SimpleMath::Vector2(1183, 632);
        
        text.font = Resources::Fonts::KG14;

        HUDText.push_back(TextRenderInfo(text));

        liveText.push_back(std::to_wstring(/*info.activeAmmo[0] +*/ info.inactiveAmmo[1]));
        text.text = liveText.at(last).c_str();
        last++;
        text.position = DirectX::SimpleMath::Vector2(1183, 428);
        text.font = Resources::Fonts::KG14;

        HUDText.push_back(TextRenderInfo(text));
    }

    //current ammo in mag of active weapon
    if (info.currentWeapon != 2)
    {
        liveText.push_back(std::to_wstring(info.activeAmmo[0]));
        text.text = liveText.at(last).c_str();
        last++;
        text.position = DirectX::SimpleMath::Vector2(750, 400);
        text.font = Resources::Fonts::KG14;

        HUDText.push_back(TextRenderInfo(text));
    }

    //time and enrage/ survive
    int minutes = info.timeRemaining / 60;
    int seconds = info.timeRemaining - (minutes*60);
    std::wstring timeString = L"";
    if (seconds < 0)
    {
        timeString = L"0:00";
        liveText.push_back(timeString);
        text.text = liveText.at(last).c_str();
        last++;
        text.position = DirectX::SimpleMath::Vector2(735, 15);
        text.font = Resources::Fonts::KG14;

        HUDText.push_back(TextRenderInfo(text));

        text.color = DirectX::SimpleMath::Color(0.9f, 0.0f, 0.3f);
        liveText.push_back(info.waveText);
        text.text = liveText.at(last).c_str();
        last++;
        text.position = DirectX::SimpleMath::Vector2(520, 15);
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
        liveText.push_back(timeString);
        text.text = liveText.at(last).c_str();
        last++;
        text.position = DirectX::SimpleMath::Vector2(735, 15);
        text.font = Resources::Fonts::KG14;

        HUDText.push_back(TextRenderInfo(text));

        text.color = DirectX::SimpleMath::Color(0.0f, 0.0f, 0.0f);
        liveText.push_back(info.waveText);
        text.text = liveText.at(last).c_str();
        last++;
        text.position = DirectX::SimpleMath::Vector2(520, 15);
        text.font = Resources::Fonts::KG14;

        HUDText.push_back(TextRenderInfo(text));
    }
    
    //wave counter
    text.color = DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f);
    liveText.push_back(std::to_wstring(info.wave) + L"/" + std::to_wstring(info.maxWaves));
    text.text = liveText.at(last).c_str();
    text.position = DirectX::SimpleMath::Vector2(660, 15);
    text.font = Resources::Fonts::KG14;

    HUDText.push_back(TextRenderInfo(text));
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
        float x = 26.f / 1024;
        float y = 258.0f / 1024;
        float width = 311.0f / 1024;
        float height = 171.0f / 1024;
        HPBar.push_back(Sprite(Sprite::BOTTOM_LEFT, Sprite::BOTTOM_LEFT, 100 + (HPBar.size() * 45), -90, 40, 35, Resources::Textures::Gamesheet, FloatRect({ x, y }, { x + width, y + height })));
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
}

void HUDManager::renderTextElements() const
{
    for (auto &text : HUDText)
    {
        QueueRender(text);
    }
}

void HUDManager::update(Player const &player, WaveTimeManager const &timeManager,
    EntityManager const &entityManager)
{
    //updates hudInfo with the current info
    info.score = ComboMachine::Get().getTotalScore();
    info.hp = player.getHP();
    info.activeAmmo[HUDManager::CURRENT_AMMO]   = player.getActiveAmmoContainer().getAmmoInfo().magAmmo;// TODO GET AMMO
    info.activeAmmo[HUDManager::TOTAL_AMMO]     = player.getActiveAmmoContainer().getAmmoInfo().enhancedAmmo;// TODO GET AMMO
    info.inactiveAmmo[HUDManager::CURRENT_AMMO] = player.getInactiveAmmoContainer().getAmmoInfo().magAmmo;// TODO GET AMMO
    info.inactiveAmmo[HUDManager::TOTAL_AMMO]   = player.getInactiveAmmoContainer().getAmmoInfo().enhancedAmmo;// TODO GET AMMO
    info.sledge = player.isUsingMeleeWeapon();
    info.currentWeapon = player.getCurrentWeapon();

    //skill cooldowns are inverted for some reason 
    const Skill* secondary = player.getSkill(SkillManager::ID::SECONDARY);
    if (!secondary->getCanUse()) {
        info.cd[0] = secondary->getCooldown() / secondary->getCooldownMax();
        info.cdInSeconds[0] = secondary->getCooldown() / 1000 + 1.f;
    }
    else
    {
        info.cd[0] = 1.0f;
        info.cdInSeconds[0] = 0;
    }
        
    const Skill* primary = player.getSkill(SkillManager::ID::PRIMARY);
    if (!primary->getCanUse()) {
        info.cd[1] = primary->getCooldown() / primary->getCooldownMax();
        info.cdInSeconds[1] = (primary->getCooldown() / 1000 ) + 1.f;
    }
    else
    {
        info.cd[1] = 1.0f;
        info.cdInSeconds[1] = 0;
    }
        
    info.wave = timeManager.getCurrentWave();
    info.maxWaves = entityManager.getWaveManager().getWaveInformation().nrOfWaves;
    if (timeManager.isEnraged())
        info.waveText = AFTER_WAVE;
    else if (timeManager.isTransitioning())
        info.waveText = BEFORE_WAVE;
    else
        info.waveText = IN_WAVE;

    info.timeRemaining = (timeManager.getTimeRequired() - timeManager.getTimeCurrent()) * 0.001f;
    info.enemiesRemaining = static_cast<int> (entityManager.getNrOfAliveEnemies());

    info.currentSkills[0] = player.getCurrentSkill0();
    info.currentSkills[1] = player.getCurrentSkill1();

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

    skillChoosen = false;
    HUDElements.clear();
    skillList.clear();
    HPBar.clear();
    skillMasks.clear();
    staticElements.clear();
    HUDText.clear();
    constructGUIElements();
}
