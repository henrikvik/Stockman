#include <Misc\ComboMachine.h>

using namespace Logic;

// Flat reward, no combo involved
void ComboMachine::Reward(int score)
{
	m_Score += score;
}

// Gives the player score depending on enemy type
void ComboMachine::Kill(EnemyType type)
{
	CheckCombo();
	m_Score += GetReward(type) * m_Combo;
}

// Needs to be updated because we need to check the combo timer
void ComboMachine::Update(float deltaTime)
{
	m_TimeSinceLastKill -= deltaTime;
}

// Reset all variables, should be called every level-restart
void ComboMachine::Reset()
{
	m_TimeSinceLastKill = 0.f;
	m_Combo = 1;
	m_Score = 0;
}

// Returns a value between 0-100, representing the time left of combo-timer, where zero is the time when the combo ends
int ComboMachine::GetComboTimer()
{
	int procent = (int)std::round((m_TimeSinceLastKill / COMBO_TIMER) / 100.f);
	return procent;
}

// Returns the player's current combo of score
int ComboMachine::GetCurrentCombo()
{
	return m_Combo;
}

// Returns the player's overall score
int ComboMachine::GetCurrentScore()
{
	return m_Score;
}

// Raises the combo if time between each kill is fast enough
void ComboMachine::CheckCombo()
{
	if (m_TimeSinceLastKill > 0.f)
	{
		m_Combo++;
		m_TimeSinceLastKill = COMBO_TIMER;
	}
	else
	{
		m_Combo = 1;
		m_TimeSinceLastKill = COMBO_TIMER;
	}
}

// Reads all the rewards from each enemy
void ComboMachine::ReadEnemyBoardFromFile(std::string path)
{
	for (int i = 0; i < 40; i++)
		m_Board[i] = 5;
}

// Gets the price on each enemy's EnemyType
int ComboMachine::GetReward(EnemyType type)
{
	return m_Board[static_cast<int> (type)];
}