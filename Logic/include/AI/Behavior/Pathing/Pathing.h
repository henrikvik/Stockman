#ifndef SIMPLE_PATHING_H
#define SIMPLE_PATHING_H

#include <vector>
#include <Entity\Entity.h>
#include <SimpleMath.h>

namespace Logic
{
	class SimplePathing
	{
	private:
		std::vector<const DirectX::SimpleMath::Vector3*> m_path;
		int m_currentNode;
	public:
		SimplePathing();

		/*
		This updates the path if on last node
		and returns the current node as a bullet vector.
		( used for moment )

		If custom behavior is wanted use other methods not this.
		*/
		const btVector3 updateAndReturnCurrentNode(
			Entity const &from, Entity const &to);

		void loadPath(Entity const &from, Entity const &to);
		std::vector<const DirectX::SimpleMath::Vector3*>& getPath();

		const DirectX::SimpleMath::Vector3* getNode() const;
		void setCurrentNode(int currentNode);
		int getCurrentNode() const;

		bool pathIsEmpty() const;
		bool pastLastNode() const;
	};
}

#endif