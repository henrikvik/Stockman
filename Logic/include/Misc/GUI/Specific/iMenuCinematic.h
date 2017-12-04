#ifndef iMENUCINEMATIC_H
#define iMENUCINEMATIC_H

#include <Misc\GUI\iMenu.h>

/*

    Only for getting better footages for the trailer.
    So yeah, it's lots of shit code. Without comments.
    Is not interfering with the rest of the program at all.

*/

namespace Logic
{
    class iMenuCinematic : public iMenu
    {
    public:
        struct Placement
        {
            DirectX::SimpleMath::Vector3 position;
            DirectX::SimpleMath::Vector3 forward;
            int index;
        };

        enum CinematicMode
        {
            Cinematic_Editing, Cinematic_Viewing
        };

        iMenuCinematic(iMenu::MenuGroup group);
        ~iMenuCinematic();

        void update(int x, int y, float deltaTime);
        void render() const;

    private:
        
        // Editing tools
        void updateEdit();
        void capture();
        void removeLast();
        void printPositions();
        void mouseMovement();
        DirectX::SimpleMath::Vector2 getWindowMidPoint();

        // Cinematic helpers
        void updatePlay();
        void play();
        void easeTo();

        bool hideImGui;
        bool hideInfo;
        int currentIndex;
        Placement* current;

        // Camera Variables
        DirectX::SimpleMath::Vector3 position;
        DirectX::SimpleMath::Vector3 forward;
        float m_camYaw;
        float m_camPitch;

        // Mode
        CinematicMode mode;
        bool switchedMode;

        // Information
        std::vector<TextRenderInfo> m_infoText;
        std::vector<std::wstring> edit_text;
        std::vector<std::wstring> viewing_text;

        std::vector<Placement> m_placements;
    }; 
}

#endif // !iMENUGAMEOVER_H
