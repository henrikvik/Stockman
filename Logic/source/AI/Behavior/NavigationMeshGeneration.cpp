#include <AI\Behavior\NavigationMeshGeneration.h>
#include <stdio.h>
#include <cmath>
#include <Physics\Physics.h>
#include <Engine\DebugWindow.h>
#include <Entity\StaticObject.h>
#include <assert.h>

#define EPSILON 0.001f

using namespace Logic;

NavigationMeshGeneration::NavigationMeshGeneration()
{
    presicion = 0.003f;
    DebugWindow::getInstance()->registerCommand("AI_NAV_SET_PRESICION", [&](std::vector<std::string> &para) -> std::string {
        try {
            presicion = std::stof(para.at(0));
            return "Presicion set to " + para.at(0);
        } catch (std::exception ex) {
            return "Set presicion, not GET presicion.";
        }
    });
}

NavigationMeshGeneration::~NavigationMeshGeneration()
{
}

void NavigationMeshGeneration::registerGenerationCommand(
    NavigationMesh &nav, Physics &physics)
{
    DebugWindow::getInstance()->registerCommand("AI_LOAD_NAV_MESH_OLD",
        [&](std::vector<std::string>) -> std::string {
        generateNavMeshOld(nav, {}, {});
        return "Generated Nav Mesh (!OLD, STATIC & DEPREACETED!)";
    });
    DebugWindow::getInstance()->registerCommand("AI_LOAD_NAV_MESH",
        [&](std::vector<std::string>) -> std::string {
        generateNavigationMesh(nav, physics);
        return "Generated Nav Mesh (NOT FULLY IMPLEMENTED!!!!!)";
    });
}

// TESTING TESTING TESTING DO NOT CALL
// ACTUALLY I WONT DO PASVF, THE CLASS WILL BE RENAMED IN THE FUTURE I FOUND A SIMPLE, PROLLY WORSE, SOLUTION :<
// THIS SHOULD ONLY BE CALLED OFFLINE AND THEN SAVED TO A FILE (TODO)
void NavigationMeshGeneration::generateNavMeshOld(NavigationMesh &nav,
    std::vector<Triangle> terrain,
    std::vector<NavStaticObject> objects) const
{
#define T 50.f // size
#define Y 0.f // node y position
    Triangle t;
    std::vector<Triangle> moveableTerrain;

    for (int z = -6; z < 6; z++)
    {
        for (int x = -6; x < 6; x++)
        {
            t.vertices[0] = DirectX::SimpleMath::Vector3(T * x, Y, T * z);
            t.vertices[1] = DirectX::SimpleMath::Vector3(T * (x + 1), Y, T * (z + 1));
            t.vertices[2] = DirectX::SimpleMath::Vector3(T * x, Y, T * (z + 1));
            terrain.push_back(t);

            t.vertices[1] = DirectX::SimpleMath::Vector3(T * (x + 1), Y, T * z);
            t.vertices[2] = DirectX::SimpleMath::Vector3(T * (x + 1), Y, T * (z + 1));
            terrain.push_back(t);
        }
    }

	DirectX::SimpleMath::Vector3 up = { 0, 1, 0 };
	float normalDotMin = 0.6f;

	// part 1 of the generation, make terrain into a "walkable" terrain map
	for (auto &triangle : terrain)
	{
		triangle.normal = getNormal(triangle);
		triangle.normal.Normalize();
		if (abs(triangle.normal.Dot(up)) > normalDotMin)
			moveableTerrain.push_back(triangle); // if triangle is too steep, enemy wont be able to go up it
	}

	// part 2, use the objects break the moveable terrain
	

	// part ??? of the generation, add it to the nav mesh because it is done :)
	for (auto &triangle : moveableTerrain) 
	{
		nav.addTriangle(
			{
				0,
				{
					triangle.vertices[0],
					triangle.vertices[1],
					triangle.vertices[2]
				}
		});
	}
    nav.createNodesFromTriangles();
    nav.generateEdges();
}

void NavigationMeshGeneration::generateNavigationMesh(NavigationMesh &nav,
    Physics &physics)
{
#define MAX_DISTANCE_ON_SIDE 150.f
    float y = 0.5f;
    std::vector<NavMeshCube> regions;
    regions.push_back(Cube({ 0.f, y, 0.f }, { 0.f, 0.f, 0.f }, { 0.2f, 0.2f, 0.2f }));

    float distance = 0;
    bool collided[SIDES] = { false, false, false, false };

    Growth growth[SIDES] = {
        { { - presicion, 0.f, 0.f }, { presicion, 0.f, 0.f } },
        { { presicion, 0.f, 0.f   }, { presicion, 0.f, 0.f } },
        { { 0.f, 0.f, - presicion }, { 0.f, 0.f, presicion } },
        { { 0.f, 0.f, presicion   }, { 0.f, 0.f, presicion } }
    };

    btCollisionObject *obj;
    btRigidBody *shape;
    StaticObject *staticObj;

    // first cube
    Cube &cube = regions[0];
    for (int j = 0; j < SIDES; j++)
    {
        distance = 0;
        while (!collided[j] && distance < MAX_DISTANCE_ON_SIDE)
        {
            cube.setDimensions(cube.getDimensions() + growth[j].dimensionChange);
            cube.setPos(cube.getPos() + growth[j].positionChange);

            shape = physics.createBody(cube, 0.f);
            for (int i = 0; i < physics.getNumCollisionObjects() && !collided[j]; i++)
            {
                obj = physics.getCollisionObjectArray()[i];

                NavContactResult res(
                    [&](btBroadphaseProxy* proxy) -> bool {
                    return true;
                },
                    [&](btManifoldPoint& cp,
                        const btCollisionObjectWrapper* colObj0, int partId0, int index0,
                        const btCollisionObjectWrapper* colObj1, int partId1, int index1) -> btScalar
                {
                    if (staticObj = dynamic_cast<StaticObject*> (reinterpret_cast<PhysicsObject*> (obj->getUserPointer())))
                        if (!(staticObj->getNavFlags() & StaticObject::NavigationMeshFlags::CULL))
                            collided[j] = handleCollision(cube, staticObj, growth[j]);
                    return 0;
                });

                physics.contactPairTest(shape, obj, res);
            }
            removeRigidBody(shape, physics);
            distance += presicion * 2;
        }
    }
    std::pair<Triangle, Triangle> triPair = toTriangle(cube);

    /* TESTING */
    shape = physics.createBody(cube, 0.f);
    assert(abs((triPair.first.vertices[0] - DirectX::SimpleMath::Vector3(shape->getWorldTransform().getOrigin() - cube.getDimensions())).Length()) < EPSILON);
    assert(abs(triPair.first.vertices[1].x - (shape->getWorldTransform().getOrigin().x() + cube.getDimensions().x())) < EPSILON);
    assert(abs(triPair.first.vertices[1].z - (shape->getWorldTransform().getOrigin().z() - cube.getDimensions().z())) < EPSILON);
    assert(abs((triPair.first.vertices[2] - DirectX::SimpleMath::Vector3(shape->getWorldTransform().getOrigin() + cube.getDimensions())).Length()) <EPSILON);
    removeRigidBody(shape, physics);
    /* TESTING */

    nav.addTriangle(toNavTriangle(triPair.first));
    nav.addTriangle(toNavTriangle(triPair.second));

    nav.createNodesFromTriangles();
    nav.generateEdges();
}

DirectX::SimpleMath::Vector3 NavigationMeshGeneration::getNormal(
    Triangle const &triangle, VertexOrder vertexOrder) const {
	if (vertexOrder == CLOCKWISE)
		return (triangle.vertices[1] - triangle.vertices[0])
            .Cross(triangle.vertices[2] - triangle.vertices[0]);
    else
        return (triangle.vertices[0] - triangle.vertices[1])
            .Cross(triangle.vertices[0] - triangle.vertices[2]);
}

std::pair<NavigationMeshGeneration::Triangle, NavigationMeshGeneration::Triangle>
    NavigationMeshGeneration::toTriangle(Cube &cube)
{
    float y = 0;
    Triangle tri1, tri2;

    tri1.vertices[0] = DirectX::SimpleMath::Vector3(cube.getPos() - cube.getDimensions());
    tri1.vertices[2] = DirectX::SimpleMath::Vector3(cube.getPos() + cube.getDimensions());
    tri1.vertices[1] = DirectX::SimpleMath::Vector3(cube.getPos() + btVector3{cube.getDimensions().x(), y, -cube.getDimensions().z()});

    tri2.vertices[0] = DirectX::SimpleMath::Vector3(cube.getPos() - cube.getDimensions());
    tri2.vertices[2] = DirectX::SimpleMath::Vector3(cube.getPos() + cube.getDimensions());
    tri2.vertices[1] = DirectX::SimpleMath::Vector3(cube.getPos() + btVector3{-cube.getDimensions().x(), y, cube.getDimensions().z()});
     
    return std::pair<Triangle, Triangle>(tri1, tri2);
}

NavigationMesh::Triangle NavigationMeshGeneration::toNavTriangle(Triangle const & tri)
{
    return { 0, tri.vertices[0], tri.vertices[1], tri.vertices[2] };
}

bool NavigationMeshGeneration::handleCollision(Cube &cube, StaticObject *obj, Growth const &growth)
{
        cube.setDimensions(cube.getDimensions() - growth.dimensionChange);
        cube.setPos(cube.getPos() - growth.positionChange);
    return true;
}

void NavigationMeshGeneration::removeRigidBody(btRigidBody *body, Physics &physics)
{
    physics.removeRigidBody(body);
    delete body->getMotionState();
    delete body->getCollisionShape();
    delete body;
}