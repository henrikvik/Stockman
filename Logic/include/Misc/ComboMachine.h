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

		int getComboTimer();
		int getCurrentCombo();
		int getCurrentScore();
        int getTotalScore();

	private:
        static const int MAX_COMBO;
        static const int MAX_MULTIKILL;
        static const float COMBO_TIMER;
        static const float MULTIKILL_TIMER;
		void checkCombo();
        void addMultikillScore();
        void addScore(int score);

		float		m_comboTimer;
        float       m_multikillTimer;
		int			m_combo;
        int         m_totalScore;
		int			m_score;
        int         m_multikill;
	};
}

#endif // !COMBOMACHINE_H
