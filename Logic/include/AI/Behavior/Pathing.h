#ifndef PATHING_H
#define PATHING_H

#include <vector>
#include <Entity\Entity.h>
#include <SimpleMath.h>

namespace Logic
{
	class Pathing
	{
	private:
		// a quick notice, they point to a position in the navmesh, directly to that data, so don't change const 
		std::vector<const DirectX::SimpleMath::Vector3*> m_path;
		int m_currentNode;
	public:
		Pathing();

		void setPath(std::vector <const DirectX::SimpleMath::Vector3*> const &path);
		std::vector<const DirectX::SimpleMath::Vector3*>& getPath();

		const DirectX::SimpleMath::Vector3& getNode() const;
		void setCurrentNode(int currentNode);
		int getCurrentNode() const;

		bool pathIsEmpty() const;
		bool pathOnLastNode() const;
	};
}

#endif