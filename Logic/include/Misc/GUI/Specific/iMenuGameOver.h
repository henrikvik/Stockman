#ifndef iMENUGAMEOVER_H
#define iMENUGAMEOVER_H

#include <Misc\GUI\iMenu.h>

namespace Logic
{
    class iMenuGameOver : public iMenu
    {
    public:
        iMenuGameOver(iMenu::MenuGroup group);

        void update(int x, int y, float deltaTime);
        void render() const;

    private:
        std::wstring m_pointsStr;
        TextRenderInfo m_textPointsInfo;
        SpriteRenderInfo m_scoreTexture;
    };
}

#endif // !iMENUGAMEOVER_H
