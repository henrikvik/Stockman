#ifndef iMENULOADING_H
#define iMENULOADING_H

#include <Misc\GUI\iMenu.h>
#include <Graphics\include\RenderInfo.h>
#include <Misc\Fader.h>

namespace Logic
{
    class iMenuLoading : public iMenu
    {
    public:
        iMenuLoading(iMenu::MenuGroup group);
        ~iMenuLoading();

        void update(int x, int y, float deltaTime);
        void render() const;
    
    private:
        float               m_extraLoadingTime; 
        bool                m_started;
        bool                m_loadComplete;
        std::wstring        m_string;   
        TextRenderInfo      m_textRenderInfo;   
        Fader               m_textFader;
    };
}

#endif // !iMENULOADING_H
