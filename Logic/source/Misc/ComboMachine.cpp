#include <Misc\ComboMachine.h>

using namespace Logic;

#define COMBO_MULTIKILL_SCORE       1000
#define COMBO_TIME_SCORE_MULTIPLIER 200

const int ComboMachine::MAX_COMBO = 16;
const int ComboMachine::MAX_MULTIKILL = 5;
const float ComboMachine::COMBO_TIMER = 5000.f;
const float ComboMachine::MULTIKILL_TIMER = 1000.f;

ComboMachine::ComboMachine()
{
    reset();
}

// Flat reward, no combo involved
void ComboMachine::reward(int score)
{
    m_totalScore += score;
}

// Gives the player score depending on enemy type
void ComboMachine::kill(int score)
{
    m_comboTimer = COMBO_TIMER;
    m_totalKills++;
    if (score)
    {
        //addScore(score); apparently this doesnt work?
        // add score to combo
        m_comboScore += score;
        // increase combo
        if(m_combo != MAX_COMBO)
            m_combo++;

        // multikill
        if (m_multikill != MAX_MULTIKILL)
        {
            if (m_multikillTimer > FLT_EPSILON)
                m_multikill++;
            m_multikillTimer = MULTIKILL_TIMER;
        }
        else
            m_multikillTimer = 0.f;
    }
}

// Needs to be updated because we need to check the combo timer
void ComboMachine::update(float deltaTime)
{
    if (m_multikillTimer > FLT_EPSILON)
        m_multikillTimer -= deltaTime;
    else if(m_multikill > 1)
        addMultikillScore();

    if (m_comboTimer > FLT_EPSILON)
        m_comboTimer -= deltaTime;
    else
    {
        reward(m_comboScore * m_combo);
        m_comboTimer = 0.f;
        m_combo = 0;
        m_multikill = 1;
        m_comboScore = 0;
    }  
}

// Reset all variables, should be called every level-restart
void ComboMachine::reset()
{
    m_totalKills = 0;
	m_comboTimer = 0.f;
	m_combo = 0;
    m_multikill = 1;
	m_comboScore = 0;
    m_totalScore = 0;
}

void ComboMachine::endCombo()
{
    addMultikillScore();
    reward(m_comboScore);
}

int ComboMachine::getTotalKills()
{
    return m_totalKills;
}

// Returns a value between 0-100, representing the time left of combo-timer, where zero is the time when the combo ends
int ComboMachine::getComboTimer()
{
	int procent = (int)std::round((m_comboTimer / COMBO_TIMER) * 100.f);
	return procent;
}

// Returns the player's current combo of score
int ComboMachine::getCurrentCombo()
{
	return m_combo;
}

// Returns the player's overall score
int ComboMachine::getComboScore()
{
	return m_comboScore;
}

int ComboMachine::getTotalScore()
{
    return m_totalScore;
}

void ComboMachine::addTimeBonus(float timeLeft)
{
    
    int timeScore = timeLeft * 0.001f;
    printf("%d", timeScore);
    m_totalScore += timeScore * COMBO_TIME_SCORE_MULTIPLIER;
}

float Logic::ComboMachine::getmaxComboTimer() const
{
    return COMBO_TIMER;
}

// Check if combo is still alive
void ComboMachine::checkCombo()
{

}

void ComboMachine::addMultikillScore()
{
    m_comboScore += m_multikill * COMBO_MULTIKILL_SCORE;
    m_multikill = 1;
}

void ComboMachine::addScore(int score)
{
    m_comboScore += score;
    if (m_combo != MAX_COMBO)
        m_combo++;

    if (m_multikill != MAX_MULTIKILL)
    {
        if (m_multikillTimer > FLT_EPSILON)
            m_multikill++;
        m_multikillTimer = MULTIKILL_TIMER;
    }
    else
        m_multikillTimer = 0.f;
}
