#include <Misc\GUI\iMenuLoading.h>
#include <Misc\GUI\iMenuAction.h>

#include <State.h>
#include <StateMachine\StateBuffer.h>
#include <StateMachine\StatePrimary.h>
#include <StateMachine\StateSecondary.h>
#include <StatePlaying.h>

using namespace Logic;

#define AFTER_COMPLETION_TEXT_FADE_TIME 1500.f
#define EXTRA_LOADING_SCREEN_TIME       800.f

iMenuLoading::iMenuLoading(iMenu::MenuGroup group) : iMenu(group)
{
    m_started               = false;
    m_loadComplete          = false;
    m_string                = L"Press Space to Continue";
    m_extraLoadingTime      = EXTRA_LOADING_SCREEN_TIME;

    // Setup of the textrenderinfo object
    m_textRenderInfo.color = DirectX::SimpleMath::Color(0, 0, 0, 0);
    m_textRenderInfo.font = Resources::Fonts::KG26;
    m_textRenderInfo.position = DirectX::SimpleMath::Vector2(0.2 * WIN_WIDTH, 0.9 * WIN_HEIGHT);
    m_textRenderInfo.text = m_string.c_str();
}

iMenuLoading::~iMenuLoading() { }

void iMenuLoading::update(int x, int y, float deltaTime)
{
    iMenu::update(x, y, deltaTime);

    m_extraLoadingTime -= deltaTime;
    if (!m_started && !m_isFading && m_extraLoadingTime < 0.f)
    {
        // Starts the fadeIn effect of the text
        if (!m_loadComplete)
            m_textFader.startFadeIn(AFTER_COMPLETION_TEXT_FADE_TIME);
        m_loadComplete = true;
       
        // Setting the text alpha
        m_textFader.update(deltaTime);
        float alpha = m_textFader.getCurrentPercentage();
        m_textRenderInfo.color = DirectX::SimpleMath::Color(alpha, alpha, alpha, alpha);

        // The actual loading
        if (DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::Space))
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
}

void iMenuLoading::render() const
{
    iMenu::render();

    QueueRender(m_textRenderInfo);
}