#ifndef IMENUSETTINGS_H
#define IMENUSETTINGS_H
#include <Misc\GUI\iMenu.h>

namespace Logic
{
    class iMenuSettings : public iMenu
    {
    public:
        iMenuSettings(iMenu::MenuGroup group);
        void update(int x, int y, float deltaTime);
        void render() const;
    private:
        TextRenderInfo m_textRenderInfoWindow;
        TextRenderInfo m_textRenderInfoDOF;
        TextRenderInfo m_textRenderInfoSSAO;
        TextRenderInfo m_textRenderInfoFog;
    };
}
#endif