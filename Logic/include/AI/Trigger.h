#ifndef TRIGGER_H
#define TRIGGER_H

#include <Entity\Entity.h>

namespace Logic
{
	class Trigger : public Entity
	{
		private:
			float m_cooldown;
		public:
			Trigger(float cooldown);
			virtual ~Trigger();

			void update(float deltaTime);

			void setCooldown(float cooldown);
			float getCooldown() const;
	};
}

#endif