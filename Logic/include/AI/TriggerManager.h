#ifndef TRIGGER_MANAGER_H
#define TRIGGER_MANAGER_H

#include <vector>
#include "Trigger.h"

#pragma region Comment
	/*
		CLASS: TriggerManager
		AUTHOR: Lukas Westling

		This class handles all the triggers and keeps
		track of the activated ones!

		Triggers themself don't keep track of activation
		because cache misses, dont change this!!!
	*/
#pragma endregion ClassDesc

namespace Logic
{
	class TriggerManager 
	{
		private:
			std::vector<Trigger> triggers;
		public:
			TriggerManager();
			~TriggerManager();

			void update(float deltaTime);
	};
}

#endif