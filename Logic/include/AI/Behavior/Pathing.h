#ifndef PATHING_H
#define PATHING_H

#include <vector>
#include <Entity\Entity.h>
#include <SimpleMath.h>

namespace Graphics
{
    struct RenderDebugInfo;
}

namespace Logic
{
	class Pathing
	{
	private:
		// a quick notice, they point to a position in the navmesh, directly to that data, so don't change const 
		std::vector<const DirectX::SimpleMath::Vector3*> m_path;
		int m_currentNode;

		Graphics::RenderDebugInfo* m_debugInfo;
	public:
		Pathing();
		Pathing(Pathing const &other) = delete;
		Pathing* operator=(Pathing const &other) = delete;
		virtual ~Pathing();

		void setPath(std::vector <const DirectX::SimpleMath::Vector3*> const &path);
		std::vector<const DirectX::SimpleMath::Vector3*>& getPath();

		const DirectX::SimpleMath::Vector3& getNode() const;
		void setCurrentNode(int currentNode);
		int getCurrentNode() const;

		bool pathIsEmpty() const;
		bool pathOnLastNode() const;

		// debugging - move to a debug render class or something maybe ty
		void initDebugRendering();
		void renderDebugging(Graphics::Renderer &renderer, DirectX::SimpleMath::Vector3 &startPosition);
	};
}

#endif