#ifndef IMENU_H
#define IMENU_H

#include <Misc\NonCopyable.h>
#include <Graphics\include\RenderQueue.h>
#include <Misc\GUI\Button.h>

namespace Logic
{
    class iMenu : public NonCopyable
    {
    public:
        enum FadingStyle { FadeIn, FadeOut };
        struct Fader
        {
            void startFadeIn(float timeInMs)
            {
                m_complete      = false;
                m_threshold     = timeInMs;
                m_start         = timeInMs;
                m_timer         = 0;
                m_style         = FadeIn;
            }

            void startFadeOut(float timeInMs)
            {
                m_complete      = false;
                m_threshold     = 0;
                m_timer         = timeInMs;
                m_start         = timeInMs;
                m_style         = FadeOut;
            }

            void update(float deltaTime)
            {
                if (m_complete)
                    return;

                // Fade in calculations
                if (m_style == FadeIn)
                {
                    m_timer += deltaTime;
                    if (m_timer > m_threshold) m_complete = true;
                }

                // Fad out calculations
                if (m_style == FadeOut) 
                {
                    m_timer -= deltaTime;
                    if (m_timer < m_threshold) m_complete = true;
                }
            }

            float getCurrentProcent()
            {
                float result = 0.f;
                
                // Calculate the alpha value roughly
                if (m_style == FadeIn)  result = m_timer / m_threshold;
                if (m_style == FadeOut) result = m_timer / m_start;
               
                // Clamping between (0:1)
                if (result < 0.f) result = 0.f;
                if (result > 1.f) result = 1.f;
               
                return result;
            }

            FadingStyle     m_style;
            bool            m_complete;
            float           m_timer;
            float           m_threshold;
            float           m_start;
        };

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

        void setDrawMenu(bool shouldDraw) { m_drawMenu = shouldDraw; }
        void setDrawButtons(bool shouldDraw) { m_drawButtons = shouldDraw; }

        bool getIsFading() { return m_isFading; }
        bool getIsSafeToRemove() { return m_safeToRemove; }
        MenuGroup getMenuType() { return m_group; }

    protected:
        void updateClick(int x, int y);
        void updateHover(int x, int y);

        Fader m_fader;
        bool m_safeToRemove;
        bool m_isFading;

        bool m_pressed;
        bool m_drawButtons;
        bool m_drawMenu;
        MenuGroup m_group;
        std::vector<Button> m_buttons;
        SpriteRenderInfo m_background;
    };
}

#endif // !MENU_H
