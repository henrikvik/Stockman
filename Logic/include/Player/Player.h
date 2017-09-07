#ifndef PLAYER_H
#define PLAYER_H
#pragma region ClassDesc
		/*
			CLASS: WeaponManager
			AUTHOR:

			DESCRIPTION: TODO
		*/
#pragma endregion

#include "Entity\Entity.h"
#include "ActionManager.h"

namespace Logic
{
	class Player : public Entity
	{
	private:
		ActionManager m_actionManager;
	public:
		Player();
		~Player();

		void clear();
		void update();
		void onCollision(const Entity& other);

	};

}

#endif // !PLAYER_H
