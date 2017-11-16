#include <Misc\GUI\iMenuCards.h>
#include <Misc\Card.h>

using namespace Logic;

#define MAX_CARD_INDEX 2

iMenuCards::iMenuCards(iMenu::MenuGroup group)
    : iMenu(group)
{

}

iMenuCards::~iMenuCards() { }

void iMenuCards::update(int x, int y, float deltaTime)
{
    iMenu::update(x, y, deltaTime);
}

void iMenuCards::setCardInformation(Card card, int cardIndex)
{
    if (cardIndex < 0)              { printf("No, fuck off.\n"); return; }
    if (cardIndex > MAX_CARD_INDEX) { printf("No, fuck off.\n"); return; }

    buildTextRenderInfo(cardIndex, card.getName(), card.getDescription(), card.getCategory());
}

void iMenuCards::render() const
{
    iMenu::render();

    for (int i = 0; i < 3; i++)
    {
        QueueRender(card[i].title);
        QueueRender(card[i].description);
    }
}

void iMenuCards::buildTextRenderInfo(int index, std::string name, std::string description, Card::CardCategory category)
{

}