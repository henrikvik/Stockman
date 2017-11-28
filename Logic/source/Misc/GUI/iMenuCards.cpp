#include <Misc\GUI\iMenuCards.h>
#include <Misc\Card.h>
#include <Misc\Sound\NoiseMachine.h>

using namespace Logic;

#define MAX_CARDS 3

iMenuCards::iMenuCards(iMenu::MenuGroup group)
    : iMenu(group) 
{ 
    m_renderable = false;
   
    // Setup of the "round complete" sprite
    m_spriteRenderInfo.alpha = 0.f;
    m_spriteRenderInfo.isMoveable = true;
    m_spriteRenderInfo.texture = Resources::Textures::WaveComplete;
    m_spriteRenderInfo.screenRect = FloatRect({
        0.307f,
        0.104f,
        512.f   / WIN_WIDTH,
        128.f   / WIN_HEIGHT
    });
    m_spriteRenderInfo.textureRect = FloatRect({ 0.0f, 0.0f }, { 1.0f, 1.0f });

    Sound::NoiseMachine::Get().playSFX(Sound::SFX::UPGRADE_UNLOCKED, nullptr, true);
}

iMenuCards::~iMenuCards() { }

void iMenuCards::update(int x, int y, float deltaTime)
{
    iMenu::update(x, y, deltaTime);

    m_spriteRenderInfo.alpha = m_fader.getCurrentPercentage();
}

void iMenuCards::setCardInformation(std::vector<Card*> inCards)
{
    if (inCards.size() != MAX_CARDS) { printf("Please insert the correct number of cards you jackass.\n"); return; }

    for (int i = 0; i < inCards.size(); i++)
    {
        Card* card = inCards[i];
        buildTextRenderInfo(i, card->getName(), card->getDescription(), card->getCategory());
        buildCardTexture(i, card->getCategory());
    }

    m_renderable = true;
}

void iMenuCards::render() const
{
    //// We only render if the cards have been created
    //if (m_renderable)
    //{
    //    for (int i = 0; i < MAX_CARDS; i++)
    //    {
    //        QueueRender(m_cardGraphic[i].title);
    //        QueueRender(m_cardGraphic[i].description);
    //        QueueRender(m_cardGraphic[i].description1);
    //    }
    //}

    // For some unkown reason, UI is taking care of this instead. Ok.

    QueueRender(m_spriteRenderInfo);
}

void iMenuCards::buildTextRenderInfo(int index, std::string name, std::string description, Card::CardCategory category)
{

    // Set fonts
    m_cardGraphic[index].title.font = Resources::Fonts::KG18;
    m_cardGraphic[index].description.font = Resources::Fonts::KG14;
    m_cardGraphic[index].description1.font = Resources::Fonts::KG14;

    // Set positions
    m_cardGraphic[index].title.position = DirectX::SimpleMath::Vector2(WIN_WIDTH / 2  - 260 + 230 * index, WIN_HEIGHT / 2- 130);
    m_cardGraphic[index].description.position = DirectX::SimpleMath::Vector2(WIN_WIDTH / 2 - 320 + 250 * index, WIN_HEIGHT / 2);
    m_cardGraphic[index].description1.position = DirectX::SimpleMath::Vector2(WIN_WIDTH / 2 - 320 + 250 * index, WIN_HEIGHT / 2 + 20);

    // Set texts



    std::wstring temp(name.begin(), name.end());


    title[index] = temp;
    m_cardGraphic[index].title.text = title[index].c_str();

    std::wstring temp1(description.begin(), description.end());
    desc[index] = temp1.substr(0, 19);
    desc1[index] = temp1.substr(19, description.length());
    m_cardGraphic[index].description.text = desc[index].c_str();
    m_cardGraphic[index].description1.text = desc1[index].c_str();

    // Set colors
    m_cardGraphic[index].title.color = DirectX::SimpleMath::Color(1.f, 1.f, 1.f, 1.f);
    m_cardGraphic[index].description.color = DirectX::SimpleMath::Color(1.f, 1.f, 1.f, 0.9f);
    m_cardGraphic[index].description1.color = DirectX::SimpleMath::Color(1.f, 1.f, 1.f, 0.9f);
}

void iMenuCards::buildCardTexture(int index, Card::CardCategory category)
{
    switch (category)
    {
    case Logic::Card::ATTACK:
        this->m_buttons.at(index).setUVS(FloatRect{ { 0.05288f, 0.0f },{ 0.34375f, 1.f } });
       
        break;
    case Logic::Card::DEFENCE:
        this->m_buttons.at(index).setUVS(FloatRect{ { 0.345352f, 0.0f },{ 0.636217f, 1.f } });
        break;
    case Logic::Card::UTILITY:
        this->m_buttons.at(index).setUVS(FloatRect{ { 0.637820f, 0.0f },{ 0.933493f, 1.f } });
        break;
    default:
        break;
    }
    

}
