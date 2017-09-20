#ifndef TRIGGER_H
#define TRIGGER_H

#include <Entity\Entity.h>

#pragma region ClassDesc
	/*
		CLASS: Trigger (Bad name TODO: change)
		AUTHOR: Lukas Westling

		This class is a Trigger, with its attributes.
		(Not activated flag, that is in the TriggerManager
		due to cachelines)
	*/
#pragma endregion

namespace Logic
{
	class Trigger : public Entity
	{
		private:
			float m_cooldown;
		public:
			Trigger(btRigidBody* body, btVector3 halfExtent, float cooldown);
			virtual ~Trigger();

			void update(float deltaTime);

			void setCooldown(float cooldown);
			float getCooldown() const;
	};
}

#endif