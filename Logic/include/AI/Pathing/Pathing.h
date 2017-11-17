#ifndef PATHING_H
#define PATHING_H

#include <Graphics\include\RenderQueue.h>

#include <vector>
#include <Entity\Entity.h>

namespace Logic
{
	class Pathing
	{
	private:
		// a quick notice, they point to a position in the navmesh, directly to that data, so don't change const 
		std::vector<const DirectX::SimpleMath::Vector3*> m_path;
		int m_currentNode;

		DebugRenderInfo m_debugInfo;
        void initDebugRendering();
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
		void renderDebugging(DirectX::SimpleMath::Vector3 &startPosition);
	};
}

#endif