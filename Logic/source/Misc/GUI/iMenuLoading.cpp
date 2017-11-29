#include <Misc\GUI\iMenuLoading.h>
#include <Misc\GUI\iMenuAction.h>

#include <State.h>
#include <StateMachine\StateBuffer.h>
#include <StateMachine\StatePrimary.h>
#include <StateMachine\StateSecondary.h>
#include <StatePlaying.h>

using namespace Logic;

#define POST_LOAD_MESSAGE               L"Press Space to Continue"  // Will fade in, after the loading is completed
#define POST_LOAD_MESSAGE_POS_X         0.2  
#define POST_LOAD_MESSAGE_POS_Y         0.9  
#define POST_LOAD_MESSAGE_FADE_TIME     3000.f                      // The time to fade-in the "Press Space to Continue" text

#define EXTRA_LOADING_SCREEN_TIME       250.f                       // Extra timer for the loading screen (Yes, I know, cancer)

/**************************************
** Loading Screen
**  Before Load & During
********************************/
iMenuLoadingPre::iMenuLoadingPre(iMenu::MenuGroup group) : iMenu(group)
{
    m_started               = false;
    m_extraLoadingTime      = EXTRA_LOADING_SCREEN_TIME;

    // Plays game start sound
    Sound::NoiseMachine::Get().playSFX(Sound::SFX::START_GAME, nullptr, true);
}

iMenuLoadingPre::~iMenuLoadingPre() { }

void iMenuLoadingPre::update(int x, int y, float deltaTime)
{
    iMenu::update(x, y, deltaTime);

    m_extraLoadingTime -= deltaTime;
    if (!m_started && !m_isFading && m_extraLoadingTime < 0.f)
    {
        // Changes the main state
        if (Action::Get().m_stateBuffer->currentPrimaryState)
            if (StatePrimary* primary = dynamic_cast<StatePrimary*>(Action::Get().m_stateBuffer->currentPrimaryState))
                primary->queueState(StateType::State_Playing);

        // Disables the secondary state (Not like it's being used, but whatever)
        if (Action::Get().m_stateBuffer->currentSecondaryState)
            if (StateSecondary* secondary = dynamic_cast<StateSecondary*>(Action::Get().m_stateBuffer->currentSecondaryState))
                secondary->queueState(StateType::Nothing);

        m_started = true;
    }
}

/**************************************
** Loading Screen 
**  Post Load
********************************/
iMenuLoadingPost::iMenuLoadingPost(iMenu::MenuGroup group) : iMenu(group)
{
    m_firstTimeHighDelta        = false;
    m_startTextFade             = false;
    m_string                    = POST_LOAD_MESSAGE;

    // Setup of the textrenderinfo object
    m_textRenderInfo.color      = DirectX::SimpleMath::Color(0, 0, 0, 0);
    m_textRenderInfo.font       = Resources::Fonts::KG26;
    m_textRenderInfo.position   = DirectX::SimpleMath::Vector2(POST_LOAD_MESSAGE_POS_X * WIN_WIDTH, POST_LOAD_MESSAGE_POS_Y * WIN_HEIGHT);
    m_textRenderInfo.text       = m_string.c_str();
}

iMenuLoadingPost::~iMenuLoadingPost() { }

// Overwritten, to avoid the actual fade-in of this screen
void iMenuLoadingPost::fadeIn()
{
    m_queingNext    = false;
    m_isFading      = false;
    m_safeToRemove  = false;
    setAlpha(1.f);
}

void iMenuLoadingPost::update(int x, int y, float deltaTime)
{
    // Exit the update loop instantly, the first time.
    //  This is done, because after the inlitializing of the full game is completed, 
    //  The deltaTime have been stacked up to a big number, 
    //  And that will in turn, fuck up things like, the fading in this class
    if (!m_firstTimeHighDelta)
    {
        m_firstTimeHighDelta = true;
        return;
    }

    iMenu::update(x, y, deltaTime);

    // Starts the fadeIn effect of the text
    if (!m_startTextFade)
        m_textFader.startFadeIn(POST_LOAD_MESSAGE_FADE_TIME);
    m_startTextFade = true;

    // Setting the text alpha
    m_textFader.update(deltaTime);
    float alpha = m_textFader.getCurrentPercentage();
    m_textRenderInfo.color = DirectX::SimpleMath::Color(alpha, alpha, alpha, alpha);

    // Press space to continue into the game
    if (DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::Space) && !m_queingNext)
    {    
        Sound::NoiseMachine::Get().playSFX(Sound::SFX::UI_BUTTON_PRESS, nullptr, true);
        Action::Get().m_menuMachine->queueMenu(iMenu::MenuGroup::Skill);
        m_queingNext = true;
    }
}

void iMenuLoadingPost::render() const
{
    iMenu::render();

    QueueRender(m_textRenderInfo);
}