#ifndef iMENUCARDS_H
#define iMENUCARDS_H

#include <Misc\GUI\iMenu.h>
#include <Graphics\include\RenderInfo.h>
#include <Misc\Card.h>
#include <string>

namespace Logic
{
    class iMenuCards : public iMenu
    {
    public:
        struct CardGraphical
        {
            TextRenderInfo title;
            TextRenderInfo description;
            TextRenderInfo description1;
        };

        struct LiveString
        {
            std::wstring title;
            std::wstring desc;
        };

        iMenuCards(iMenu::MenuGroup group);
        ~iMenuCards();

        void setCardInformation(std::vector<Card*> cards);

        void update(int x, int y, float deltaTime);
        void render() const;

    private:
        void buildTextRenderInfo(int index, std::string name, std::string description, Card::CardCategory category);
        void buildCardTexture(int index, Card::CardCategory category);

        bool                m_renderable;
        CardGraphical       m_cardGraphic[3];
        std::wstring        title[3];
        std::wstring        desc[3];
        std::wstring        desc1[3];
        SpriteRenderInfo    m_spriteRenderInfo; //< The Wave Completed Box
    };
}

#endif // !iMENUCARDS_H