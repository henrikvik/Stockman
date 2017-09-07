#ifndef MENUMACHINE_H
#define MENUMACHINE_H

#include <vector>
#include "Misc\Enums.h"
#include "Entity\Object.h"

namespace Logic
{
	class MenuMachine
	{
	public:
		MenuMachine();
		~MenuMachine();

		void showMenu(GameState state);
		void removeMenu();
	private:
		std::vector<Object> m_buttons;

		void readMenuLayoutFromFile();
	};
}

#endif // !MENUMACHINE_H
