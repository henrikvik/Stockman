#ifndef IMENUSETTINGS_H
#define IMENUSETTINGS_H
#include <Misc\GUI\iMenu.h>

namespace Logic
{
    class iMenuSettings : public iMenu
    {
    public:
        iMenuSettings(iMenu::MenuGroup group);
        ~iMenuSettings();
        void update(int x, int y, float deltaTime);
        void render() const;
    private:
        bool currentWindowed;
    protected:
        void confirm();
    };
}
#endif