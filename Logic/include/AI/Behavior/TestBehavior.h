#ifndef TEST_BEHAVIOR_H
#define TEST_BEHAVIOR_H

#include "Behavior.h" 

namespace Logic 
{
	class TestBehavior : public Behavior
	{
	private:
		SimplePathing m_path;
		Graphics::RenderDebugInfo debugInfo;
	public:
		TestBehavior();
		virtual ~TestBehavior();

		virtual void update(Enemy &enemy, std::vector<Enemy*> const &close, Player const &player, float deltaTime);
		virtual void updatePath(Entity const &from, Entity const &to);
		virtual void debugRendering(Graphics::Renderer &renderer);
	};
}

#endif