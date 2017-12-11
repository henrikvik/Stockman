#include <Misc\GUI\Specific\iMenuHighscore.h>
#include <Misc\Network\dbConnect.h>
#include <comdef.h> // For use of _bstr_t
#include <Keyboard.h>

using namespace Logic;

// Realtime editing of the colum's and row's positioning
#define ENTRY_POS_EDIT      false

// Input the varibles gotten from the editing here
#define ENTRY_POS_X         0.308f
#define ENTRY_POS_Y         0.160f
#define ENTRY_POS_Y_OFFSET  0.071f
#define COLUMN_2_OFFSET     0.104f
#define COLUMN_3_OFFSET     0.316f
#define COLUMN_4_OFFSET     0.444f
#define COLUMN_5_OFFSET     0.570f

// Max length of name that can be outputted on screen, to avoid clustering
#define MAX_NAME_LENGTH     12

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

    for (size_t i = 1; i < 11; i++)
    {
        EntrySpot* spot = newd EntrySpot();
        spot->filledWithData = false;

        spot->renderInfoPlacing.color = OTHER_PLACE_COLOR;
        spot->renderInfoPlacing.font = Resources::Fonts::nordic;
        spot->renderInfoPlacing.position = DirectX::SimpleMath::Vector2(ENTRY_POS_X * WIN_WIDTH, (ENTRY_POS_Y + (ENTRY_POS_Y_OFFSET * i)) * WIN_HEIGHT);
        spot->renderInfoName.text = i;

        // Entry Name Render Texture
        spot->renderInfoName.color = OTHER_PLACE_COLOR;
        spot->renderInfoName.font = Resources::Fonts::nordic;
        spot->renderInfoName.position = DirectX::SimpleMath::Vector2((ENTRY_POS_X + COLUMN_2_OFFSET) * WIN_WIDTH, (ENTRY_POS_Y + (ENTRY_POS_Y_OFFSET * i)) * WIN_HEIGHT);
        spot->renderInfoName.text = L"";

        // Entry Score Render Texture
        spot->renderInfoScore.color = OTHER_PLACE_COLOR;
        spot->renderInfoScore.font = Resources::Fonts::nordic;
        spot->renderInfoScore.position = DirectX::SimpleMath::Vector2((ENTRY_POS_X + COLUMN_3_OFFSET)* WIN_WIDTH, (ENTRY_POS_Y + (ENTRY_POS_Y_OFFSET * i)) * WIN_HEIGHT);
        spot->renderInfoScore.text = L"";

        // Entry Time Render Texture
        spot->renderInfoTime.color = OTHER_PLACE_COLOR;
        spot->renderInfoTime.font = Resources::Fonts::nordic;
        spot->renderInfoTime.position = DirectX::SimpleMath::Vector2((ENTRY_POS_X + COLUMN_4_OFFSET) * WIN_WIDTH, (ENTRY_POS_Y + (ENTRY_POS_Y_OFFSET * i)) * WIN_HEIGHT);
        spot->renderInfoTime.text = L"";

        // Entry Time Render Texture
        spot->renderInfoKills.color = OTHER_PLACE_COLOR;
        spot->renderInfoKills.font = Resources::Fonts::nordic;
        spot->renderInfoKills.position = DirectX::SimpleMath::Vector2((ENTRY_POS_X + COLUMN_5_OFFSET) * WIN_WIDTH, (ENTRY_POS_Y + (ENTRY_POS_Y_OFFSET * i)) * WIN_HEIGHT);
        spot->renderInfoKills.text = L"";

        m_spot.push_back(spot);
    }
}

iMenuHighscore::~iMenuHighscore() 
{
    clearEntries();

    for (size_t i = 0; i < m_spot.size(); i++)
    {
        delete m_spot[i];
        m_spot[i] = nullptr;
    }
    m_spot.clear();
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
    std::vector<std::vector<std::string>> entries = db.getHigscoreStats(500);

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

    buildSpots(0);
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
    entry->time     = std::to_wstring(entry->data.time) + L"";
    entry->score    = std::to_wstring(entry->data.score) + L"";
    entry->kills    = std::to_wstring(entry->data.kills) + L"";

    m_entry.push_back(entry);
}

void iMenuHighscore::buildSpot(EntrySpot* spot, Entry* entry)
{
    // Convert placement to int 
    int position = _wtoi(entry->placing.c_str());

    // Entry Placing Render Texture
    if      (position == 1) spot->renderInfoPlacing.color = FIRST_PLACE_COLOR;
    else if (position == 2) spot->renderInfoPlacing.color = SECOND_PLACE_COLOR;
    else if (position == 3) spot->renderInfoPlacing.color = THIRD_PLACE_COLOR;
    else                    spot->renderInfoPlacing.color = OTHER_PLACE_COLOR;
    spot->renderInfoPlacing.text = entry->placing.c_str();

    // Replace with correct entry intofmration
    spot->renderInfoName.text = entry->name.c_str();
    spot->renderInfoScore.text = entry->score.c_str();
    spot->renderInfoTime.text = entry->time.c_str();
    spot->renderInfoKills.text = entry->kills.c_str();

    // Set as active
    spot->filledWithData = true;
}

void iMenuHighscore::buildSpots(int startIndex)
{
    for (size_t i = 0; i < m_spot.size(); i++)
        m_spot[i]->filledWithData = false;

    for (size_t i = 0; i < m_spot.size(); i++)
        if (m_entry.size() > (i + startIndex))
            buildSpot(m_spot[i], m_entry[i + startIndex]);
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

    static int start        = 0;
    static boolean pressed  = true;
    if (!pressed && DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::Down))
    {
        start += 10;
        pressed = true;
        buildSpots(start);
    }
    else if (!pressed && DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::Up))
    {
        start -= 10;
        if (start < 0) start = 0;
        pressed = true;
        buildSpots(start);
    }
    
    if (!DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::Up) &&
        !DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::Down))
        pressed = false;
}

void iMenuHighscore::render() const
{
    iMenu::render();

    for (size_t i = 0; i < m_spot.size(); i++)
    {
        EntrySpot* spot = m_spot[i];
        if (spot->filledWithData)
        {
            QueueRender(spot->renderInfoName);
            QueueRender(spot->renderInfoScore);
            QueueRender(spot->renderInfoPlacing);
            QueueRender(spot->renderInfoTime);
            QueueRender(spot->renderInfoKills);
        }
    }
}