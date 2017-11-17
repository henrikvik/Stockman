#ifndef IMENU_H
#define IMENU_H

#include <Misc\NonCopyable.h>
#include <Graphics\include\RenderQueue.h>
#include <Misc\GUI\Button.h>
#include <Misc\Fader.h>

namespace Logic
{
    class iMenu : public NonCopyable
    {
    public:
        enum MenuGroup
        {
            Empty,
            Intro,
            Start,
            Settings,
            Skill,
            CardSelect,
            Highscore,
            GameOver
        };

        struct ButtonData
        {
            FloatRect                   screenRect;     // Where the button should be drawn
            FloatRect                   texRectNormal;  // The texture-coordinates on the button-map
            FloatRect                   texRectHover;   // The texture-coordinates on the button-map
            FloatRect                   texRectActive;  // The texture-coordinates on the button-map
            Resources::Textures::Files  texture;        // What buttonmap-texture this button is present inside
            std::function<void(void)>   callback;       // What function this button calls
        };

        iMenu(MenuGroup group);
        ~iMenu();

        void fadeIn();
        void fadeOut();

        void addBackground(Resources::Textures::Files texture, float alpha);
        void addButton(ButtonData btn);
        virtual void update(int x, int y, float deltaTime);
        virtual void render() const;

        void setDrawMenu(bool shouldDraw)       { m_drawMenu    = shouldDraw;   }
        void setDrawButtons(bool shouldDraw)    { m_drawButtons = shouldDraw;   }
        void setAlpha(float alpha);

        bool getIsFading()                      { return m_isFading;        }
        bool getIsSafeToRemove()                { return m_safeToRemove;    }
        MenuGroup getMenuType()                 { return m_group;           }

    protected:
        void updateClick(int x, int y);
        void updateHover(int x, int y);

        // Fading out/in
        Fader                   m_fader;
        bool                    m_safeToRemove;
        bool                    m_isFading;
        float                   m_fadingTimer;

        // Menu
        SpriteRenderInfo        m_background;
        std::vector<Button>     m_buttons;
        bool                    m_pressed;
        MenuGroup               m_group;
        DirectX::Mouse::Mode    m_mouseMode;

        // Hide menu
        bool                    m_drawButtons;
        bool                    m_drawMenu;
    };
}

#endif // !MENU_H
