#include <Misc\GUI\iMenuHighscore.h>
#include <comdef.h>

using namespace Logic;

#define ENTRY_POS_X 0.301f
#define ENTRY_POS_Y 0.167f
#define ENTRY_POS_Y_OFFSET 0.068f

#define COLUMN_2_OFFSET 0.074
#define COLUMN_3_OFFSET 0.299
#define COLUMN_4_OFFSET 0.466

#define ENTRY_POS_EDIT false

iMenuHighscore::iMenuHighscore(iMenu::MenuGroup group)
    : iMenu(group) 
{
    m_highscoreManager.loadFromFile();
    buildHighscore();
}

iMenuHighscore::~iMenuHighscore() { }

void iMenuHighscore::buildHighscore()
{
    std::vector<HighScoreManager::HighScore> entries = m_highscoreManager.getHighScores();
    for (size_t i = 0; i < 10 /*entries.size()*/; i++)
    {
        /* 
        TEMP
        */
        HighScoreManager::HighScore score;
        score.name = "Stockman";
        score.score = 11;

        buildEntry(i, score);
    }
}

void iMenuHighscore::buildEntry(int position, HighScoreManager::HighScore stat)
{
    Entry entry;
    entry.stats = stat;

    // Entry Placing
    entry.renderInfoPlacing.color = DirectX::SimpleMath::Color(1, 1, 1, 1);
    entry.renderInfoPlacing.font = Resources::Fonts::KG18;
    entry.renderInfoPlacing.position = DirectX::SimpleMath::Vector2(ENTRY_POS_X * WIN_WIDTH, (ENTRY_POS_Y + (ENTRY_POS_Y_OFFSET * position)) * WIN_HEIGHT);
    entry.renderInfoPlacing.text = _bstr_t(std::to_string(position).c_str());
   
    // Entry Name
    entry.renderInfoName.color = DirectX::SimpleMath::Color(1, 1, 1, 1);
    entry.renderInfoName.font = Resources::Fonts::KG18;
    entry.renderInfoName.position = DirectX::SimpleMath::Vector2((ENTRY_POS_X + COLUMN_2_OFFSET) * WIN_WIDTH, (ENTRY_POS_Y + (ENTRY_POS_Y_OFFSET * position)) * WIN_HEIGHT);
    entry.renderInfoName.text = _bstr_t(entry.stats.name.c_str());

    // Entry Score
    entry.renderInfoScore.color = DirectX::SimpleMath::Color(1, 1, 1, 1);
    entry.renderInfoScore.font = Resources::Fonts::KG18;
    entry.renderInfoScore.position = DirectX::SimpleMath::Vector2((ENTRY_POS_X + COLUMN_3_OFFSET)* WIN_WIDTH, (ENTRY_POS_Y + (ENTRY_POS_Y_OFFSET * position)) * WIN_HEIGHT);
    entry.renderInfoScore.text = _bstr_t(std::to_string(entry.stats.score).c_str());

    // Entry Time
    entry.renderInfoTime.color = DirectX::SimpleMath::Color(1, 1, 1, 1);
    entry.renderInfoTime.font = Resources::Fonts::KG18;
    entry.renderInfoTime.position = DirectX::SimpleMath::Vector2((ENTRY_POS_X + COLUMN_4_OFFSET)* WIN_WIDTH, (ENTRY_POS_Y + (ENTRY_POS_Y_OFFSET * position)) * WIN_HEIGHT);
    entry.renderInfoTime.text = _bstr_t(std::to_string(entry.stats.score).c_str());

    m_entry.push_back(entry);
}

void iMenuHighscore::update(int x, int y, float deltaTime)
{
    iMenu::update(x, y, deltaTime);

#if ENTRY_POS_EDIT

    static float posx;
    static float posy;
    static float posYoffset;

    static float col2;
    static float col3;
    static float col4;

    if (ImGui::Begin("Edit"))
    {
        ImGui::DragFloat("X", &posx, 0.0001f, 0, 1);
        ImGui::DragFloat("Y", &posy, 0.0001f, 0, 1);
        ImGui::DragFloat("Offset", &posYoffset, 0.0001f, 0, 1);
        ImGui::DragFloat("Col 2 Offset", &col2, 0.0001f, 0, 1);
        ImGui::DragFloat("Col 3 Offset", &col3, 0.0001f, 0, 1);
        ImGui::DragFloat("Col 4 Offset", &col4, 0.0001f, 0, 1);

        ImGui::End();
    }

    for (size_t i = 0; i < 10 /*entries.size()*/; i++)
    {
        m_entry[i].renderInfoPlacing.position = DirectX::SimpleMath::Vector2(posx * WIN_WIDTH, (posy + (posYoffset * i)) * WIN_HEIGHT);
        m_entry[i].renderInfoName.position = DirectX::SimpleMath::Vector2((posx + col2) * WIN_WIDTH, (posy + (posYoffset * i)) * WIN_HEIGHT);
        m_entry[i].renderInfoScore.position = DirectX::SimpleMath::Vector2((posx + col3) * WIN_WIDTH, (posy + (posYoffset * i)) * WIN_HEIGHT);
        m_entry[i].renderInfoTime.position = DirectX::SimpleMath::Vector2((posx + col4) * WIN_WIDTH, (posy + (posYoffset * i)) * WIN_HEIGHT);
    }
#endif
}

void iMenuHighscore::render() const
{
    iMenu::render();

    for (size_t i = 0; i < m_entry.size(); i++)
    {
        QueueRender(m_entry[i].renderInfoName);
        QueueRender(m_entry[i].renderInfoScore);
        QueueRender(m_entry[i].renderInfoPlacing);
        QueueRender(m_entry[i].renderInfoTime);
    }
}