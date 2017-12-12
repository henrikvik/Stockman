#ifndef iMENUHIGHSCORE_H
#define iMENUHIGHSCORE_H

#include <Misc\GUI\iMenu.h>
#include <Graphics\include\RenderInfo.h>
#include <Misc\Network\dbConnect.h>

namespace Logic
{
    class iMenuHighscore : public iMenu
    {
    public:
        struct HigscoreData
        {
            HigscoreData() : name(""), score(0), time(0), wave(0), kills(0) { }
            HigscoreData(std::string msg) : name(msg), score(0), time(0), wave(0), kills(0) { }

            int         score;
            int         time;
            int         wave;
            int         kills;
            std::string name;
        };

        struct Entry
        {
            HigscoreData data;

            std::wstring name;
            std::wstring placing;
            std::wstring score;
            std::wstring time;
            std::wstring kills;
        };

        struct EntrySpot
        {
            bool filledWithData;

            TextRenderInfo              renderInfoName;
            TextRenderInfo              renderInfoPlacing;
            TextRenderInfo              renderInfoScore;
            TextRenderInfo              renderInfoTime;
            TextRenderInfo              renderInfoKills;
        };

        iMenuHighscore(iMenu::MenuGroup group);
        ~iMenuHighscore();

        void clearEntries();

        void buildHighscore();
        void buildEntry(int position, HigscoreData data);

        void buildSpot(EntrySpot* spot, Entry* data);
        void buildSpots(int startIndex);

        void update(int x, int y, float deltaTime);
        void render() const;

        void up();
        void down();

    private:
        void updateScrolling();

        bool                    m_requestDone;
        std::vector<Entry*>     m_entry;
        std::vector<EntrySpot*> m_spot;
        int start;
    };
}

#endif // !iMENUHIGHSCORE_H