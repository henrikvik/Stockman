#include <AI\Behavior\PASVF.h>
#define GET_NORMAL(x)
using namespace Logic;

PASVF::PASVF()
{
}

PASVF::~PASVF()
{
}

NavigationMesh PASVF::generateNavMesh(int x, int y, int width, int height, std::vector<Object> collidableObjects) const
{
	struct Polygon {
		std::vector<DirectX::SimpleMath::Vector2> points;
		std::vector<DirectX::SimpleMath::Vector2> edges;
	};
	NavigationMesh nav;

	std::vector<Polygon> expanding;
	std::vector<Polygon> done;
	
	for (auto &polygon : expanding) 
	{

	}

	return nav;
}
