#ifndef COMBOMACHINE_H
#define COMBOMACHINE_H

#include <AI\EnemyType.h>
#include <string>

namespace Logic
{
	class ComboMachine
	{
	public:
		const int	MAX_COMBO	= 16;
		const float COMBO_TIMER	= 5000.f;

		static ComboMachine& Get()
		{
			static ComboMachine combo;
			return combo;
		}

		void Reward(int score);
		void Kill(EnemyType type);
		void Update(float deltaTime);
		void Reset();

		int GetComboTimer();
		int GetCurrentCombo();
		int GetCurrentScore();

		void ReadEnemyBoardFromFile(std::string path);

	private:
		void CheckCombo();
		int GetReward(EnemyType type);

		float		m_TimeSinceLastKill;
		int			m_Combo;					
		int			m_Score;
		int			m_Board[40];
	};
}

#endif // !COMBOMACHINE_H
