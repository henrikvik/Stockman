#ifndef TRIGGER_H
#define TRIGGER_H

#include <Player\Player.h>
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
		public:
			Trigger(btRigidBody* body, btVector3 halfExtent, float cooldown);
			virtual ~Trigger();

			void addUpgrades(const std::vector<StatusManager::UPGRADE_ID>& upgrades);
			void addEffects(const std::vector<StatusManager::EFFECT_ID>& effects);

			void update(float deltaTime);
			void onCollision(Entity& other);

			bool getIsActive() const;
			bool getIsReusable() const;
			float getCooldown() const;
			void setIsActive(bool active);
			void setIsReusable(bool	reusable);
			void setCooldown(float cooldown);

		private:
			bool m_active;
			bool m_reusable;
			float m_cooldown;
	};
}

#endif