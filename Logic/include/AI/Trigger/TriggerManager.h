#ifndef TRIGGER_MANAGER_H
#define TRIGGER_MANAGER_H

#include <vector>
#include "Trigger.h"
#include <Physics\Primitives.h>

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

            void reset();

			void removeTrigger(Trigger* t, int index);

			Trigger* addTrigger(Trigger::TriggerType type, btVector3 const &pos, Physics& physics,
                std::vector<StatusManager::UPGRADE_ID> upgrades, 
                std::vector<StatusManager::EFFECT_ID> effects);

			void update(float deltaTime);
			void render() const;
	
			std::vector<Trigger*>& getTriggers();
	};
}

#endif