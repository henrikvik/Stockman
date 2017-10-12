#ifndef COMBOMACHINE_H
#define COMBOMACHINE_H

#include <AI\Enemy.h>

namespace Logic
{
	class ComboMachine
	{
	public:
		const int MAX_COMBO		= 16;
		const int COMBO_TIMER	= 5000.f;

		static ComboMachine& Get()
		{
			static ComboMachine combo;
			return combo;
		}

		void Reward(int score);
		void Kill(Enemy::ENEMY_TYPE type);
		void Update(float deltaTime);
		void Reset();

		int GetComboTimer();
		int GetCurrentCombo();
		int GetCurrentScore();

		void ReadEnemyBoardFromFile(std::string path);

	private:
		void CheckCombo();
		int GetReward(Enemy::ENEMY_TYPE type);

		float		m_TimeSinceLastKill;
		int			m_Combo;					
		int			m_Score;
		int			m_Board[40];
	};
}

#endif // !COMBOMACHINE_H
