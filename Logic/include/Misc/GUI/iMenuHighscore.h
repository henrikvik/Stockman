#ifndef iMENUHIGHSCORE_H
#define iMENUHIGHSCORE_H

#include <Misc\GUI\iMenu.h>
#include <Misc\HighScoreManager.h>
#include <Graphics\include\RenderInfo.h>

namespace Logic
{
    class iMenuHighscore : public iMenu
    {
    public:
        struct Entry
        {
            HighScoreManager::HighScore stats;

            TextRenderInfo              renderInfoName;
            TextRenderInfo              renderInfoPlacing;
            TextRenderInfo              renderInfoScore;
            TextRenderInfo              renderInfoTime;
        };

        iMenuHighscore(iMenu::MenuGroup group);
        ~iMenuHighscore();

        void buildHighscore();
        void buildEntry(int position, HighScoreManager::HighScore stat);

        void update(int x, int y, float deltaTime);
        void render() const;

    private:
        std::vector<Entry>          m_entry;
        HighScoreManager            m_highscoreManager;
    };
}

#endif // !iMENUHIGHSCORE_H