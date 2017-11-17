#include <Misc\GUI\iMenuCards.h>
#include <Misc\Card.h>
#include <Misc\StringConverter.h>

using namespace Logic;

#define MAX_CARDS 3

iMenuCards::iMenuCards(iMenu::MenuGroup group)
    : iMenu(group) 
{ 
    m_renderable = false;
}

iMenuCards::~iMenuCards() { }

void iMenuCards::update(int x, int y, float deltaTime)
{
    iMenu::update(x, y, deltaTime);

    // Card animations
    // here?
}

void iMenuCards::setCardInformation(std::vector<Card*> inCards)
{
    if (inCards.size() != MAX_CARDS) { printf("Please insert the correct number of cards you jackass.\n"); return; }

    for (int i = 0; i < inCards.size(); i++)
    {
        Card* card = inCards[i];
        buildTextRenderInfo(i, card->getName(), card->getDescription(), card->getCategory());
    }

    m_renderable = true;
}

void iMenuCards::render() const
{
    iMenu::render();

    // We only render if the cards have been created
    if (m_renderable)
    {
        for (int i = 0; i < MAX_CARDS; i++)
        {
            QueueRender(m_cardGraphic[i].title);
            QueueRender(m_cardGraphic[i].description);
        }
    }
}

void iMenuCards::buildTextRenderInfo(int index, std::string name, std::string description, Card::CardCategory category)
{
    // Set fonts
    m_cardGraphic[index].title.font = Resources::Fonts::KG18;
    m_cardGraphic[index].description.font = Resources::Fonts::KG14;

    // Set positions
    m_cardGraphic[index].title.position = DirectX::SimpleMath::Vector2(0.5f, 0.5f);
    m_cardGraphic[index].description.position = DirectX::SimpleMath::Vector2(0.5f, 0.5f);

    // Set texts
    m_cardGraphic[index].title.text = convertToWString(name).c_str();
    m_cardGraphic[index].description.text = convertToWString(description).c_str();

    // Set colors
    m_cardGraphic[index].title.color = DirectX::SimpleMath::Color(1.f, 1.f, 1.f, 1.f);
    m_cardGraphic[index].description.color = DirectX::SimpleMath::Color(1.f, 1.f, 1.f, 0.9f);
}
