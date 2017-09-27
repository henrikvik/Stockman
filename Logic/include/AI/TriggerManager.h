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
			Physics* m_physicsPtr;
			std::vector<Trigger*> m_triggers;
		public:
			TriggerManager();
			~TriggerManager();

			void removeTrigger(Trigger* t, int index);

			void addTrigger(Cube& cube, float cooldown, Physics& physics, 
				std::vector<StatusManager::UPGRADE_ID> upgrades, 
				std::vector<StatusManager::EFFECT_ID> effects, 
				bool reusable = false);

			void update(float deltaTime);
			void render(Graphics::Renderer &renderer);
	
			std::vector<Trigger*>& getTriggers();
	};
}

#endif