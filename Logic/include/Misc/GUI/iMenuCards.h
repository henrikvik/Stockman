#ifndef iMENUCARDS_H
#define iMENUCARDS_H

#include <Misc\GUI\iMenu.h>
#include <Graphics\include\RenderInfo.h>
#include <Misc\Card.h>

namespace Logic
{
    class iMenuCards : public iMenu
    {
    public:
        struct CardGraphical
        {
            TextRenderInfo title;
            TextRenderInfo description;
            
        };

        iMenuCards(iMenu::MenuGroup group);
        ~iMenuCards();

        void setCardInformation(Card card, int cardIndex);

        void update(int x, int y, float deltaTime);
        void render() const;

    private:
        void buildTextRenderInfo(int index, std::string name, std::string description, Card::CardCategory category);

        CardGraphical card[3];
    };
}

#endif // !iMENUCARDS_H