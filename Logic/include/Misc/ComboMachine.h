#ifndef COMBOMACHINE_H
#define COMBOMACHINE_H

#include <AI\EnemyType.h>
#include <string>

namespace Logic
{
	class ComboMachine
	{
    public:
		static ComboMachine& Get()
		{
			static ComboMachine combo;
			return combo;
		}

        ComboMachine();

		void reward(int score);
		void kill(int score = 0);
		void update(float deltaTime);
		void reset();

        void endCombo();

        int getTotalKills();
		int getComboTimer();
		int getCurrentCombo();
		int getComboScore();
        int getTotalScore();
        void addTimeBonus(float timeLeft);

	private:
        static const int MAX_COMBO;
        static const int MAX_MULTIKILL;
        static const float COMBO_TIMER;
        static const float MULTIKILL_TIMER;
		void checkCombo();
        void addMultikillScore();
        void addScore(int score);

        int         m_totalKills;
		float		m_comboTimer;
        float       m_multikillTimer;
		int			m_combo;
        int         m_totalScore;
		int			m_comboScore;
        int         m_multikill;
	};
}

#endif // !COMBOMACHINE_H
