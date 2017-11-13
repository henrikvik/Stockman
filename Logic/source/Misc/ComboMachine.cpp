#include <Misc\ComboMachine.h>

using namespace Logic;

const int ComboMachine::MAX_COMBO = 16;
const float ComboMachine::COMBO_TIMER = 8000.f;

// Flat reward, no combo involved
void ComboMachine::reward(int score)
{
	m_score += score;
}

// Gives the player score depending on enemy type
void ComboMachine::kill(int score)
{
    m_comboTimer = COMBO_TIMER;
	m_score += score * m_combo;
}

// Needs to be updated because we need to check the combo timer
void ComboMachine::update(float deltaTime)
{
	m_comboTimer -= deltaTime;
}

// Reset all variables, should be called every level-restart
void ComboMachine::reset()
{
	m_comboTimer = 0.f;
	m_combo = 1;
	m_score = 0;
}

// Returns a value between 0-100, representing the time left of combo-timer, where zero is the time when the combo ends
int ComboMachine::getComboTimer()
{
	int procent = (int)std::round((m_comboTimer / COMBO_TIMER) / 100.f);
	return procent;
}

// Returns the player's current combo of score
int ComboMachine::getCurrentCombo()
{
	return m_combo;
}

// Returns the player's overall score
int ComboMachine::getCurrentScore()
{
	return m_score;
}

// Raises the combo if time between each kill is fast enough
void ComboMachine::checkCombo()
{
	if (m_comboTimer > 0.f)
	{
		m_combo++;
		m_comboTimer = COMBO_TIMER;
	}
	else
	{
		m_combo = 1;
		m_comboTimer = COMBO_TIMER;
	}
}