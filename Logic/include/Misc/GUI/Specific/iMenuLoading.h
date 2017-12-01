#ifndef iMENULOADING_H
#define iMENULOADING_H

#include <Graphics\include\RenderInfo.h>
#include <Misc\GUI\iMenu.h>
#include <Misc\Fader.h>

/*

    The LoadingScreenPre "menu", loads in the next state by calling the primary state
    The LoadingScreenPost is just a buffer, needs a keypress from the user to switch into the game

    - We need to split this class up into two classes because of various reasons

*/

namespace Logic
{
    /**************************************
    ** Loading Screen
    **  Before Load & During
    ********************************/
    class iMenuLoadingPre : public iMenu
    {
    public:
        iMenuLoadingPre(iMenu::MenuGroup group);
        ~iMenuLoadingPre();

        void update(int x, int y, float deltaTime);

    private:
        float               m_extraLoadingTime; // The amount in ms to prolong the loading screen
        bool                m_started;          // When the loading of the next state have started
    };

    /**************************************
    ** Loading Screen
    **  Post Load
    ********************************/
    class iMenuLoadingPost : public iMenu
    {
    public:
        iMenuLoadingPost(iMenu::MenuGroup group);
        ~iMenuLoadingPost();

        void fadeIn();
        void update(int x, int y, float deltaTime);
        void render() const;
    
    private:
        bool                m_queingNext;
        bool                m_firstTimeHighDelta;
        bool                m_startTextFade;    // If we've started to fade-in the text or not
        std::wstring        m_string;           // The "Press Space to Continue" text 
        TextRenderInfo      m_textRenderInfo;   // The graphical parts of the text
        Fader               m_textFader;        // We use this to do the actual fading of the text
    };
}

#endif // !iMENULOADING_H
