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

        struct LiveString
        {
            std::wstring title;
            std::wstring desc;
        };

        iMenuCards(iMenu::MenuGroup group);

        void update(int x, int y, float deltaTime);
        void render() const;

    private:

        bool                m_renderable;
        SpriteRenderInfo    m_spriteRenderInfo; //< The Wave Completed Box
    };
}

#endif // !iMENUCARDS_H