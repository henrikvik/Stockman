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

		void reward(int score);
		void kill(int score = 0);
		void update(float deltaTime);
		void reset();

		int getComboTimer();
		int getCurrentCombo();
		int getCurrentScore();

	private:
        static const int	MAX_COMBO;
        static const float COMBO_TIMER;
		void checkCombo();

		float		m_comboTimer;
		int			m_combo;					
		int			m_score;
	};
}

#endif // !COMBOMACHINE_H
