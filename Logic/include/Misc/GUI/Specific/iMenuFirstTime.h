#ifndef iMENUFIRSTTIME_H
#define iMENUFIRSTTIME_H

#include <Misc\GUI\iMenu.h>
#include <Graphics\include\RenderInfo.h>

namespace Logic
{
    class iMenuFirstTime : public iMenu
    {
    public:
        iMenuFirstTime(iMenu::MenuGroup group);
        ~iMenuFirstTime();

        void confirm();
        void update(int x, int y, float deltaTime);
        void render() const;
   
    private:

        std::wstring m_textConfirm;
        std::wstring m_textMessage;
        std::wstring m_textInput;
        TextRenderInfo m_textInfoConfirm;
        TextRenderInfo m_textInfoMessage;
        TextRenderInfo m_textInfoInput;
    };
}

#endif // !iMENUFIRSTTIME_H