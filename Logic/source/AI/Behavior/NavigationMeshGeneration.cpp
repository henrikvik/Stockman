#include <AI\Behavior\NavigationMeshGeneration.h>
#include <stdio.h>
#include <cmath>
#include <Physics\Physics.h>
#include <Engine\DebugWindow.h>
#include <Entity\StaticObject.h>
#include <assert.h>
#include <Physics\FuncContactResult.h>

#define EPSILON 0.001f

using namespace Logic;

NavigationMeshGeneration::NavigationMeshGeneration()
{
    precision = 5.05f;
    maxLength = 150.f;
    baseY = 0.5f;

    registerSetCommands();

    growth[X_PLUS] = { { precision, 0.f, 0.f }, { precision, 0.f, 0.f } }; // X +
    growth[Z_MINUS] = { { 0.f, 0.f, -precision },{ 0.f, 0.f, precision } }; // Z-
    growth[X_MINUS] = { { -precision , 0.f, 0.f },{ precision, 0.f, 0.f } }; // X-
    growth[Z_PLUS] = { { 0.f, 0.f, precision },{ 0.f, 0.f, precision } }; // Z+

    growthNormals[X_PLUS] = { 1.f, 0.f, 0.f };
    growthNormals[Z_MINUS] = { 0.f, 0.f, -1.f };
    growthNormals[X_MINUS] = { -1.f, 0.f, 0.f };
    growthNormals[Z_PLUS] = { 0.f, 0.f, 1.f };
}

NavigationMeshGeneration::~NavigationMeshGeneration()
{
}

void NavigationMeshGeneration::registerSetCommands()
{
    DebugWindow::getInstance()->registerCommand("AI_NAV_SET_PRECISION", [&](std::vector<std::string> &para) -> std::string {
        try {
            precision = std::stof(para.at(0));
            return "Presicion set to " + para.at(0);
        }
        catch (std::exception ex) {
            return "Set presicion, not GET presicion.";
        }
    });
    DebugWindow::getInstance()->registerCommand("AI_NAV_SET_MAX_LEN", [&](std::vector<std::string> &para) -> std::string {
        try {
            maxLength = std::stof(para.at(0));
            return "Max length set to " + para.at(0);
        }
        catch (std::exception ex) {
            return "When life gives you lemon, (parameter wrong)";
        }
    });
    DebugWindow::getInstance()->registerCommand("AI_NAV_SET_BASE_Y", [&](std::vector<std::string> &para) -> std::string {
        try {
            baseY = std::stof(para.at(0));
            return "Base Y set to " + para.at(0);
        }
        catch (std::exception ex) {
            return "Make lemonade. (parameter wrong)";
        }
    });
}

void NavigationMeshGeneration::registerGenerationCommand(
    NavigationMesh &nav, Physics &physics)
{
    DebugWindow::getInstance()->registerCommand("AI_NAV_LOAD_MESH_OLD",
        [&](std::vector<std::string>) -> std::string {
        nav.clear();
        generateNavMeshOld(nav, {}, {});
        return "Generated Nav Mesh (!OLD, STATIC & DEPREACETED!)";
    });
    DebugWindow::getInstance()->registerCommand("AI_NAV_LOAD_MESH",
        [&](std::vector<std::string>) -> std::string {
        nav.clear();
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
    std::vector<NavMeshCube> regions;
    regions.reserve(100); // THIS IS A TEMPORARY SOLUTION TO PREVENT MEMORY FOK UPS; MAKE A REAL SOLUTION
    regions.push_back(NavMeshCube(Cube({ 0.f, baseY, 0.f }, { 0.f, 0.f, 0.f }, { 0.2f, 0.2f, 0.2f })));
    regions.push_back(NavMeshCube(Cube({ -80.f, baseY, 80.f }, { 0.f, 0.f, 0.f }, { 0.2f, 0.2f, 0.2f })));
    regions.push_back(NavMeshCube(Cube({ 60.f, baseY, 140.f }, { 0.f, 0.f, 0.f }, { 0.2f, 0.2f, 0.2f })));

    float distance;

    btCollisionObject *obj;
    StaticObject *staticObj;

    // first cube
    printf("Buckleup buckero this will take a while! Generating Navigation Mesh...\n");
    for (size_t regionIndex = 0; regionIndex < regions.size() && regionIndex < 10; regionIndex++) // less than ten to prevent yikes
    {
        printf("Loading.. %d/%d\n", static_cast<int> (regionIndex + 1), static_cast<int> (regions.size()));
        auto &region = regions[regionIndex];
        for (int side = 0; side < SIDES && !region.collided[side]; side++)
        {
            distance = 0;
            while (!region.collided[side] && distance < maxLength)
            {
                region.cube.setDimensions(region.cube.getDimensions() + growth[side].dimensionChange);
                region.cube.setPos(region.cube.getPos() + growth[side].positionChange);

                region.body = physics.createBody(region.cube, 0.f);
                for (int i = 0; i < physics.getNumCollisionObjects() && !region.collided[side]; i++)
                {
                    obj = physics.getCollisionObjectArray()[i];

                    FunContactResult res(
                        [&](btBroadphaseProxy* proxy) -> bool {
                        if (!proxy->isConvex2d(TRIANGLE_MESH_SHAPE_PROXYTYPE) || !proxy->isConvex(TRIANGLE_MESH_SHAPE_PROXYTYPE))
                        {
                            printf("WARNING: NON CONVEX OBJECT!\nNAV MESH GENERATION UNDEFINED BEHAVIOR!\n");
                            return false;
                        }
                        return true;
                    },
                        [&](btManifoldPoint& cp,
                            const btCollisionObjectWrapper* colObj0, int partId0, int index0,
                            const btCollisionObjectWrapper* colObj1, int partId1, int index1) -> btScalar
                    {
                        if (cp.getDistance() > precision) return 0;
                        if (staticObj = dynamic_cast<StaticObject*> (reinterpret_cast<PhysicsObject*> (obj->getUserPointer())))
                        {
                            if (!(staticObj->getNavFlags() & StaticObject::NavigationMeshFlags::CULL))
                            {
                                if (btBoxShape* bs = dynamic_cast<btBoxShape*>(staticObj->getRigidBody()->getCollisionShape())) // only support box shapes at the moment (other shapes can be "converted" to boxes)
                                {
                                    cp.m_localPointA += region.body->getWorldTransform().getOrigin();
                                    CollisionReturn ret = handleCollision(cp.m_localPointA, region, staticObj, growth[side], growthNormals[side], bs);
                                    switch (ret)
                                    {
                                    case ON_VERTEX:
                                            split(regions, physics, region, cp.m_localPointA, growthNormals[side + 1 % SIDES]); // clockwise rotation
                                        break;
                                    }
                                    if (ret != PROBLEMS_MY_DUDES) region.collided[side] = true;
                                }
                            }
                        }
                        else if (obj->getUserIndex() > 0 && &regions[obj->getUserIndex()] != region.buddy) // use base class or someting later
                        {
                            // instead of skipping buddies, couple them or something
                            region.cube.setDimensions(region.cube.getDimensions() - growth[side].dimensionChange);
                            region.cube.setPos(region.cube.getPos() - growth[side].positionChange);
                            region.collided[side] = true;
                        }
                        return 0;
                    });
                    physics.contactPairTest(region.body, obj, res);
                }

                removeRigidBody(region.body, physics);
                distance += precision * 2;
            }
        }
        region.done = true;
        region.body = physics.createBody(region.cube, 0.f);
        region.body->setUserIndex(static_cast<int> (regionIndex));
    }

    for (auto &region : regions)
    {
        Cube &cube = region.cube;
        std::pair<Triangle, Triangle> triPair = toTriangle(cube);

        region.body = physics.createBody(cube, 0.f);
        /* TESTING */
      //  assert(abs((triPair.first.vertices[0] - DirectX::SimpleMath::Vector3(region.body->getWorldTransform().getOrigin() - cube.getDimensions())).Length()) < EPSILON);
      //  assert(abs(triPair.first.vertices[1].x - (region.body->getWorldTransform().getOrigin().x() + cube.getDimensions().x())) < EPSILON);
      //  assert(abs(triPair.first.vertices[1].z - (region.body->getWorldTransform().getOrigin().z() - cube.getDimensions().z())) < EPSILON);
      //  assert(abs((triPair.first.vertices[2] - DirectX::SimpleMath::Vector3(region.body->getWorldTransform().getOrigin() + cube.getDimensions())).Length()) < EPSILON);
        /* TESTING */
        removeRigidBody(region.body, physics);

        nav.addTriangle(toNavTriangle(triPair.first));
        nav.addTriangle(toNavTriangle(triPair.second));
    }

    nav.createNodesFromTriangles();
    nav.generateEdges();
    printf("Finished! :D");
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
    cube.getDimensionsRef().setY(y);

    tri1.vertices[0] = DirectX::SimpleMath::Vector3(cube.getPos() - cube.getDimensions());
    tri1.vertices[1] = DirectX::SimpleMath::Vector3(cube.getPos() + cube.getDimensions());
    tri1.vertices[2] = DirectX::SimpleMath::Vector3(cube.getPos() + btVector3{cube.getDimensions().x(), y, -cube.getDimensions().z()});

    tri2.vertices[0] = DirectX::SimpleMath::Vector3(cube.getPos() - cube.getDimensions());
    tri2.vertices[1] = DirectX::SimpleMath::Vector3(cube.getPos() + cube.getDimensions());
    tri2.vertices[2] = DirectX::SimpleMath::Vector3(cube.getPos() + btVector3{-cube.getDimensions().x(), y, cube.getDimensions().z()});
     
    return std::pair<Triangle, Triangle>(tri1, tri2);
}

std::pair<Cube, Cube> NavigationMeshGeneration::cutCube(btVector3 const &cutPoint, btVector3 const &planeNormal, Cube const &cube)
{
    std::pair<Cube, Cube> cubes(cube, cube);
    Cube &f = cubes.first, &s = cubes.second;

    btVector3 end = cube.getPos() + cube.getDimensions();
    btVector3 toPos = cutPoint - end;
    btVector3 temp;

    float distToPlane = abs(toPos.dot(planeNormal) / planeNormal.length2()) * 0.5f;

    if (planeNormal.x() != 0)
    {
        f.getDimensionsRef().setX(distToPlane);
        s.getDimensionsRef().setX(s.getDimensionsRef().x() - distToPlane);

        temp = f.getPos();
        temp.setX(end.x() - distToPlane);
        f.setPos(temp);

        temp = s.getPos();
        temp.setX(end.x() - f.getDimensionsRef().x() * 2 - s.getDimensionsRef().x());
        s.setPos(temp);
    }
    else
    {
        f.getDimensionsRef().setZ(distToPlane);
        s.getDimensionsRef().setZ(s.getDimensionsRef().z() - distToPlane);

        temp = f.getPos();
        temp.setZ(end.z() - distToPlane);
        f.setPos(temp);

        temp = s.getPos();
        temp.setZ(end.z() - f.getDimensionsRef().z() * 2 - s.getDimensionsRef().z());
        s.setPos(temp);
    }

    return cubes;
}

NavigationMesh::Triangle NavigationMeshGeneration::toNavTriangle(Triangle const & tri)
{
    return { 0, tri.vertices[0], tri.vertices[1], tri.vertices[2] };
}

NavigationMeshGeneration::CollisionReturn NavigationMeshGeneration::handleCollision(btVector3 collisionPoint,
    NavMeshCube &cube, StaticObject *obj, Growth const &growth, btVector3 growthNormal, btBoxShape *shape)
{
    cube.cube.setDimensions(cube.cube.getDimensions() - growth.dimensionChange);
    cube.cube.setPos(cube.cube.getPos() - growth.positionChange);

    DirectX::SimpleMath::Quaternion rot = obj->getRotation();
    rot.w = 0; // this is stupid

    if (rot.LengthSquared() < EPSILON) // base case
    {
        return ON_AXIS;
    }
    
    btVector3 vertex;
    bool found = false;

    for (int i = 0; i < shape->getNumVertices() && !found; i++)
    {
        shape->getVertex(i, vertex);
        if (shape->isInside(vertex, btScalar(0.05f)))
            found = true;
    }

    if (found) // case b, on vertex
    {
        return ON_VERTEX;
    }
    else // case c, living on the edge. (Collision on edge)
    {
        return ON_EDGE;
    }

    return PROBLEMS_MY_DUDES;
}

void NavigationMeshGeneration::split(std::vector<NavMeshCube> &regions, Physics &physics,
    NavMeshCube &cube, btVector3 const &cubeColPoint, btVector3 const &splitPlaneNormal)
{
    NavMeshCube cube1(cube);
    std::pair<Cube, Cube> btCubes = cutCube(cubeColPoint, splitPlaneNormal, cube.cube);

    cube1.buddy = &cube;
    cube1.cube = btCubes.first;
    cube1.body = nullptr;
    cube.cube = btCubes.second;

    if (splitPlaneNormal.z() != 0)
    {
        cube1.collided[Z_MINUS] = true;
        cube.collided[Z_PLUS] = true;
    }
    else
    {
        cube1.collided[X_MINUS] = true;
        cube.collided[X_PLUS] = true;
    }

    cube1.body = physics.createBody(cube.cube, 0.f);
    cube1.body->setUserIndex(-666);
    
    regions.push_back(cube1);
}

void NavigationMeshGeneration::removeRigidBody(btRigidBody *body, Physics &physics)
{
    physics.removeRigidBody(body);
    delete body->getMotionState();
    delete body->getCollisionShape();
    delete body;
}

std::pair<bool, btVector3> NavigationMeshGeneration::rayTestCollisionPoint(StaticObject *obj, btRigidBody *reg, Physics &physics, btVector3 &normalIncrease, float maxDistance)
{
    bool collision;
    btVector3 point;
    btVector3 endPoint = -1.f * (normalIncrease * maxDistance);
    btRigidBody *body = obj->getRigidBody();

    // THIS IS A TESTING SOLUTION FOR THE MOMENT, CUSTOM
    btCollisionWorld::ClosestRayResultCallback res(body->getWorldTransform().getOrigin(), endPoint);

    return std::pair<bool, btVector3>(collision, point);
}
