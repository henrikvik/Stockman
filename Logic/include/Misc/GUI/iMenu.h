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
        enum MenuGroup
        {
            Start,
            Settings,
            Skill,
            Card,
            Highscore,
            GameOver
        };

        struct ButtonData
        {
            FloatRect                   screenRect; // Where the button should be drawn
            FloatRect                   texRect;    // The texture-coordinates on the button-map
            Resources::Textures::Files  texture;    // What buttonmap-texture this button is present inside
            std::function<void(void)>   callback;   // What function this button calls
        };

        iMenu(MenuGroup group);
        ~iMenu();

        void addBackground(Resources::Textures::Files texture, float alpha);
        void addButton(ButtonData btn);
        void update(int x, int y);
        void render() const;

        MenuGroup getMenuType() { return m_group; }

    private:
        void updateClick(int x, int y);
        void updateHover(int x, int y);

        MenuGroup m_group;
        std::vector<Button> m_buttons;
        SpriteRenderInfo m_background;
    };
}

#endif // !MENU_H
