#include <Misc\GUI\iMenuFirstTime.h>
#include <Misc\GUI\iMenuAction.h>
#include <Misc\GUI\iMenuMachine.h>
#include <Keyboard.h>
#include <Engine\Typing.h>
#include <Engine\Settings.h>
#include <comdef.h> // For use of _bstr_t

#define EDIT_TEXT_OBJECTS false

#define TEXT_MSG_POS_X 0.096            // Message to input name text pos x
#define TEXT_MSG_POS_Y 0.305            // Message to input name text pos y
#define TEXT_INP_CHAR_SIZE 0.004170f    // This is used to center the name
#define TEXT_INP_POS_X 0.161            // Position of the input text object
#define TEXT_INP_POS_Y 0.462            // Position of the input text object
#define TEXT_ENT_POS_X 0.108            // Message to user to press enter to confirm
#define TEXT_ENT_POS_Y 0.635            // Message to user to press enter to confirm

using namespace Logic;

iMenuFirstTime::iMenuFirstTime(iMenu::MenuGroup group)
    : iMenu(group)
{
    // Setup of the textrenderinfo object
    m_textMessage = L"Hi, I'm gonna need a name.";
    m_textInfoMessage.color = DirectX::SimpleMath::Color(0, 0, 0, 0);
    m_textInfoMessage.font = Resources::Fonts::KG14;
    m_textInfoMessage.position = DirectX::SimpleMath::Vector2(TEXT_MSG_POS_X * WIN_WIDTH, TEXT_MSG_POS_Y * WIN_HEIGHT);
    m_textInfoMessage.text = m_textMessage.c_str();

    // Setup of the textrenderinfo object
    m_textInput = L"Stockman";
    m_textInfoInput.color = DirectX::SimpleMath::Color(0, 0, 0, 0);
    m_textInfoInput.font = Resources::Fonts::KG18;
    m_textInfoInput.position = DirectX::SimpleMath::Vector2((TEXT_INP_POS_X - (m_textInput.size() * TEXT_INP_CHAR_SIZE)) * WIN_WIDTH, TEXT_INP_POS_Y * WIN_HEIGHT);
    m_textInfoInput.text = m_textInput.c_str();

    // Setup of the textrenderinfo object
    m_textConfirm = L"Press Enter to confirm.";
    m_textInfoConfirm.color = DirectX::SimpleMath::Color(0, 0, 0, 0);
    m_textInfoConfirm.font = Resources::Fonts::KG14;
    m_textInfoConfirm.position = DirectX::SimpleMath::Vector2(TEXT_ENT_POS_X * WIN_WIDTH, TEXT_ENT_POS_Y * WIN_HEIGHT);
    m_textInfoConfirm.text = m_textConfirm.c_str();
}

iMenuFirstTime::~iMenuFirstTime() { }

void iMenuFirstTime::confirm()
{
    Settings::getInstance().setName(std::string(_bstr_t(m_textInput.c_str())));
    Settings::getInstance().writeToFile();
    printf("Saving %ls as your name in Data/Settings.lw.\n", m_textInput.c_str());
    printf("Never push Settings.lw to Github.\nIf it shows up as a modification in the Github extension -> Right-Click on it and press 'Ignore this local file'.\n");
    Action::Get().m_menuMachine->queueMenu(iMenu::MenuGroup::Start);
}

void iMenuFirstTime::update(int x, int y, float deltaTime)
{
    iMenu::update(x, y, deltaTime);

    if (m_isFading)
    {
        float alpha = m_fader.getCurrentPercentage();
        m_textInfoConfirm.color = DirectX::SimpleMath::Color(alpha, alpha, alpha, alpha);
        m_textInfoInput.color = DirectX::SimpleMath::Color(alpha, alpha, alpha, alpha);
        m_textInfoMessage.color = DirectX::SimpleMath::Color(alpha, alpha, alpha, alpha);
    }

#if EDIT_TEXT_OBJECTS
    if (ImGui::Begin("Edit"))
    {
        static float x1, x2, x3;
        static float y1, y2, y3, offset;
        ImGui::SliderFloat("Text_1.x", &x1, 0.f, 1.f, "%.3f");
        ImGui::SliderFloat("Text_1.y", &y1, 0.f, 1.f, "%.3f");
        ImGui::SliderFloat("Text_2.x", &x2, 0.f, 1.f, "%.3f");
        ImGui::SliderFloat("Text_2.y", &y2, 0.f, 1.f, "%.3f");
        ImGui::SliderFloat("Text_3.x", &x3, 0.f, 1.f, "%.3f");
        ImGui::SliderFloat("Text_3.y", &y3, 0.f, 1.f, "%.3f");
        ImGui::DragFloat("Offset", &offset, 0.00001f, 0.f, 0.01f, "%.6f");

        m_textInfoMessage.position = DirectX::SimpleMath::Vector2(x1 * WIN_WIDTH, y1 * WIN_HEIGHT);
        m_textInfoInput.position = DirectX::SimpleMath::Vector2((x2 - (m_textInput.size() * offset)) * WIN_WIDTH, y2 * WIN_HEIGHT);
        m_textInfoConfirm.position = DirectX::SimpleMath::Vector2(x3 * WIN_WIDTH, y3 * WIN_HEIGHT);
        ImGui::End();
    }
#endif

    if ((DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::Enter) && !m_isFading && !m_safeToRemove))
    {
        confirm();
    }
    else
    {
        char input = Typing::getInstance()->getSymbol();
        if (input == '\b' && !m_textInput.empty())
        {
            m_textInput.pop_back();
            m_textInfoInput.text = m_textInput;
        }
        else if (!isspace(input) && isalpha(input) && m_textInput.size() < 24)
        {
            m_textInput.push_back(input);
            m_textInfoInput.text = m_textInput;
        }
        m_textInfoInput.position = DirectX::SimpleMath::Vector2((TEXT_INP_POS_X - (m_textInput.size() * TEXT_INP_CHAR_SIZE)) * WIN_WIDTH, TEXT_INP_POS_Y * WIN_HEIGHT);
    }
}

void iMenuFirstTime::render() const
{
    iMenu::render();

    QueueRender(m_textInfoConfirm);
    QueueRender(m_textInfoMessage);
    QueueRender(m_textInfoInput);
}