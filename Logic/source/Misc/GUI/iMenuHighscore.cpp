#include <Misc\GUI\iMenuHighscore.h>
#include <Misc\Network\dbConnect.h>
#include <comdef.h> // For use of _bstr_t

using namespace Logic;

// Realtime editing of the colum's and row's positioning
#define ENTRY_POS_EDIT      false

// Input the varibles gotten from the editing here
#define ENTRY_POS_X         0.308f
#define ENTRY_POS_Y         0.231f
#define ENTRY_POS_Y_OFFSET  0.071f
#define COLUMN_2_OFFSET     0.104f
#define COLUMN_3_OFFSET     0.316f
#define COLUMN_4_OFFSET     0.444f
#define COLUMN_5_OFFSET     0.570f

// Max length of name that can be outputted on screen, to avoid clustering
#define MAX_NAME_LENGTH     24

// Coloring of the placing
#define FIRST_PLACE_COLOR   DirectX::SimpleMath::Color(1, 0.843137, 0, 1)
#define SECOND_PLACE_COLOR  DirectX::SimpleMath::Color(0.745098, 0.745098, 0.745098, 1)
#define THIRD_PLACE_COLOR   DirectX::SimpleMath::Color(0.3984375, 0.18359375, 0.12890625, 1)
#define OTHER_PLACE_COLOR   DirectX::SimpleMath::Color(1, 1, 1, 1)

iMenuHighscore::iMenuHighscore(iMenu::MenuGroup group)
    : iMenu(group) 
{
    m_requestDone = false;
    buildEntry(0, HigscoreData("Loading"));
}

iMenuHighscore::~iMenuHighscore() 
{
    clearEntries();
}

void iMenuHighscore::clearEntries()
{
    for (size_t i = 0; i < m_entry.size(); i++)
    {
        delete m_entry[i];
        m_entry[i] = nullptr;
    }
    m_entry.clear();
}

void iMenuHighscore::buildHighscore()
{
    clearEntries();
    
    // Get the highscores from the database
    Network::dbConnect db;
    std::vector<std::vector<std::string>> entries = db.getHigscoreStats(10);

    if (!entries.empty())
    {
        // Reverse the list, because we get the lowest score first
        std::reverse(entries.begin(), entries.end());

        // Build the graphical representation of the highscore
        for (size_t i = 0; i < entries.size(); i++)
        {
            if (entries[i].size() == 5)
            {
                // Builds an entry of highscore stats
                HigscoreData data;
                data.name = entries[i][0];
                data.score = atoi(entries[i][1].c_str());
                data.kills = atoi(entries[i][2].c_str());
                data.wave = atoi(entries[i][3].c_str());
                data.time = atoi(entries[i][4].c_str());
                buildEntry(i, data);
            }
            else
            {
                buildEntry(i, HigscoreData("Invalid Data"));
            }
        }
    }
    else
    {
        buildEntry(0, HigscoreData("No connection to server"));
    }
}

void iMenuHighscore::buildEntry(int position, HigscoreData data)
{
    // Setup of the entry
    Entry* entry    = newd Entry();
    entry->data     = data;
    
    // No clustering in the menu's
    if (entry->data.name.length() > MAX_NAME_LENGTH)
    {
        entry->data.name.resize(MAX_NAME_LENGTH);
        entry->data.name.shrink_to_fit();
        entry->data.name += "..";
    }

    // Filling the wStrings with the data
    entry->name     = _bstr_t(entry->data.name.c_str());
    entry->placing  = std::to_wstring(position + 1);
    entry->time     = std::to_wstring(entry->data.time) + L" Seconds";
    entry->score    = std::to_wstring(entry->data.score) + L" Points";
    entry->kills    = std::to_wstring(entry->data.kills) + L" Kills";

    // Entry Placing Render Texture
    if      (position == 0) entry->renderInfoPlacing.color  = FIRST_PLACE_COLOR;
    else if (position == 1) entry->renderInfoPlacing.color  = SECOND_PLACE_COLOR;
    else if (position == 2) entry->renderInfoPlacing.color  = THIRD_PLACE_COLOR;
    else                    entry->renderInfoPlacing.color  = OTHER_PLACE_COLOR;
    entry->renderInfoPlacing.font       = Resources::Fonts::nordic;
    entry->renderInfoPlacing.position   = DirectX::SimpleMath::Vector2(ENTRY_POS_X * WIN_WIDTH, (ENTRY_POS_Y + (ENTRY_POS_Y_OFFSET * position)) * WIN_HEIGHT);
    entry->renderInfoPlacing.text       = entry->placing.c_str();
   
    // Entry Name Render Texture
    entry->renderInfoName.color         = OTHER_PLACE_COLOR;
    entry->renderInfoName.font          = Resources::Fonts::nordic;
    entry->renderInfoName.position      = DirectX::SimpleMath::Vector2((ENTRY_POS_X + COLUMN_2_OFFSET) * WIN_WIDTH, (ENTRY_POS_Y + (ENTRY_POS_Y_OFFSET * position)) * WIN_HEIGHT);
    entry->renderInfoName.text          = entry->name.c_str();

    // Entry Score Render Texture
    entry->renderInfoScore.color        = OTHER_PLACE_COLOR;
    entry->renderInfoScore.font         = Resources::Fonts::nordic;
    entry->renderInfoScore.position     = DirectX::SimpleMath::Vector2((ENTRY_POS_X + COLUMN_3_OFFSET)* WIN_WIDTH, (ENTRY_POS_Y + (ENTRY_POS_Y_OFFSET * position)) * WIN_HEIGHT);
    entry->renderInfoScore.text         = entry->score.c_str();

    // Entry Time Render Texture
    entry->renderInfoTime.color         = OTHER_PLACE_COLOR;
    entry->renderInfoTime.font          = Resources::Fonts::nordic;
    entry->renderInfoTime.position      = DirectX::SimpleMath::Vector2((ENTRY_POS_X + COLUMN_4_OFFSET) * WIN_WIDTH, (ENTRY_POS_Y + (ENTRY_POS_Y_OFFSET * position)) * WIN_HEIGHT);
    entry->renderInfoTime.text          = entry->time.c_str();

    // Entry Time Render Texture
    entry->renderInfoKills.color        = OTHER_PLACE_COLOR;
    entry->renderInfoKills.font         = Resources::Fonts::nordic;
    entry->renderInfoKills.position     = DirectX::SimpleMath::Vector2((ENTRY_POS_X + COLUMN_5_OFFSET) * WIN_WIDTH, (ENTRY_POS_Y + (ENTRY_POS_Y_OFFSET * position)) * WIN_HEIGHT);
    entry->renderInfoKills.text         = entry->kills.c_str();

    m_entry.push_back(entry);
}

void iMenuHighscore::update(int x, int y, float deltaTime)
{
    iMenu::update(x, y, deltaTime);

    if (!m_isFading) if (!m_requestDone) { m_requestDone = true; buildHighscore(); }

// Debugging purposes
#if ENTRY_POS_EDIT
    static float posx, posy, posYoffset, col2, col3, col4, col5;
    if (ImGui::Begin("Edit"))
    {
        ImGui::DragFloat("X", &posx, 0.0001f, 0, 1);
        ImGui::DragFloat("Y", &posy, 0.0001f, 0, 1);
        ImGui::DragFloat("Offset", &posYoffset, 0.0001f, 0, 1);
        ImGui::DragFloat("Col 2 Offset", &col2, 0.0001f, 0, 1);
        ImGui::DragFloat("Col 3 Offset", &col3, 0.0001f, 0, 1);
        ImGui::DragFloat("Col 4 Offset", &col4, 0.0001f, 0, 1);
        ImGui::DragFloat("Col 5 Offset", &col5, 0.0001f, 0, 1);
        ImGui::End();
    }
    for (size_t i = 0; i < m_entry.size(); i++)
    {
        m_entry[i]->renderInfoPlacing.position = DirectX::SimpleMath::Vector2(posx * WIN_WIDTH, (posy + (posYoffset * i)) * WIN_HEIGHT);
        m_entry[i]->renderInfoName.position = DirectX::SimpleMath::Vector2((posx + col2) * WIN_WIDTH, (posy + (posYoffset * i)) * WIN_HEIGHT);
        m_entry[i]->renderInfoScore.position = DirectX::SimpleMath::Vector2((posx + col3) * WIN_WIDTH, (posy + (posYoffset * i)) * WIN_HEIGHT);
        m_entry[i]->renderInfoTime.position = DirectX::SimpleMath::Vector2((posx + col4) * WIN_WIDTH, (posy + (posYoffset * i)) * WIN_HEIGHT);
        m_entry[i]->renderInfoKills.position = DirectX::SimpleMath::Vector2((posx + col5) * WIN_WIDTH, (posy + (posYoffset * i)) * WIN_HEIGHT);
    }

#endif
}

void iMenuHighscore::render() const
{
    iMenu::render();

    for (size_t i = 0; i < m_entry.size(); i++)
    {
        QueueRender(m_entry[i]->renderInfoName);
        QueueRender(m_entry[i]->renderInfoScore);
        QueueRender(m_entry[i]->renderInfoPlacing);
        QueueRender(m_entry[i]->renderInfoTime);
        QueueRender(m_entry[i]->renderInfoKills);
    }
}