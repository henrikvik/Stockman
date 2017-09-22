#include <AI\Behavior\PASVF.h>
#include <stdio.h>
using namespace Logic;

inline DirectX::SimpleMath::Vector2 Logic::PASVF::getNormal(DirectX::SimpleMath::Vector2 const & edge) const
{
	return { -edge.y, edge.x };
}

PASVF::PASVF()
{
}

PASVF::~PASVF()
{
}

// TESTING TESTING TESTING DO NOT CALL
NavigationMesh PASVF::generateNavMesh(float x, float y, int width, int height, std::vector<Entity*> collidableObjects) const
{
	struct Polygon {
		std::vector<DirectX::SimpleMath::Vector2> points;
		std::vector<DirectX::SimpleMath::Vector2> edges;
		btRigidBody* collision;
	};
	NavigationMesh nav;

	std::vector<Polygon> expanding;
	std::vector<Polygon> done;

	// FOR TESTING
	Polygon base;
	base.points.push_back({ x, y });
	base.points.push_back({ x + 1, y });
	base.points.push_back({ x + 1, y + 1 });
	base.points.push_back({ x, y + 1 });
	base.points.push_back({ x, y }); // SAME AS FIRST TO GO AROUND

	base.edges.push_back({ 1, 0 }); // 0 -> 1
	base.edges.push_back({ 0, 1 }); // 1 -> 2
	base.edges.push_back({ -1, 0 }); // 2 -> 3
	base.edges.push_back({ 0, -1 }); // 3 -> 4 / 0

	btCollisionShape* shape = new btBoxShape({ 0.5f, 0.5f, 0.5f });
	btRigidBody::btRigidBodyConstructionInfo constructionInfo(0, nullptr, shape);
	base.collision = new btRigidBody(constructionInfo);

	expanding.push_back(base);
	
	for (auto &polygon : expanding) 
	{
		for (size_t i = 0; i < polygon.edges.size(); ++i) 
		{
			DirectX::SimpleMath::Vector2 normal = getNormal(polygon.edges[i]);
			normal.Normalize();
			polygon.points[i] += normal;
			polygon.points[i + 1] += normal;

			for (auto& obj : collidableObjects)
			{
				if (obj->getRigidbody()->checkCollideWith(base.collision))
				{
					printf("Collide with %s\n", obj);
				}
			}
		}
	}

	done.push_back(base);
	for (auto &polygon : done) {
		delete polygon.collision;
	}
	delete shape;
	return nav;
}
