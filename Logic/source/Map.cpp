#include "Map.h"

#include <Keyboard.h>
#include <fstream>
#include <toml\toml.h>
#include <memory>

#include <Graphics\include\Structs.h>
#include <Graphics\include\Utility\DebugDraw.h>
#include <Graphics/include/Utility/ModelLoader.h>

#include <Physics/Physics.h>        
#include <Misc\RandomGenerator.h>
#include <Misc\Sound\SoundSource.h>

using namespace Logic;

#define AI_BOX_ID_MIN 31

static void FillLightVec(std::vector<LightRenderInfo> &lights, std::string path)
{
    auto val = toml::parseFile(path).value;

    for (auto light : val.find("lights")->as<toml::Array>()) {
        auto position = light["position"].as<toml::Array>();
        auto col = light["color"].as<toml::Array>();
        auto radius = light["range"].asNumber();
        auto intensity = light["intensity"].asNumber();

        LightRenderInfo info = {};
        info.position.x = position[0].asNumber();
        info.position.y = position[1].asNumber();
        info.position.z = position[2].asNumber();
        info.color.x = col[0].asNumber();
        info.color.y = col[1].asNumber();
        info.color.z = col[2].asNumber();
        info.range = radius;
        info.intensity = intensity;

        lights.push_back(info);
    }
}

static void SubmitLights(const std::vector<LightRenderInfo> &lights)
{
    for (auto light : lights) {
        QueueRender(light);
    }
}

Map::Map() :
    m_ChristmasLightTimer(1.5f),
    m_ChristmasPatternIndex(0)
{
    m_mapObject = std::make_unique<StaticObject*>(newd StaticObject(Resources::Models::UnitCube,
        nullptr, btVector3(0, 0, 0), StaticObject::NavigationMeshFlags::NO_CULL));

    // static map lights
    FillLightVec(m_MapLights, "../Resources/Maps/lights.toml");
    FillLightVec(m_RedBulbs, "../Resources/Maps/r.toml");
    FillLightVec(m_GreenBulbs, "../Resources/Maps/g.toml");
    FillLightVec(m_BlueBulbs, "../Resources/Maps/b.toml");

    //                                       bgr
    m_ChristmasLightPattern.push_back(0b00000000);
    m_ChristmasLightPattern.push_back(0b00000001);
    m_ChristmasLightPattern.push_back(0b00000010);
    m_ChristmasLightPattern.push_back(0b00000100);
    m_ChristmasLightPattern.push_back(0b00000001);
    m_ChristmasLightPattern.push_back(0b00000011);
    m_ChristmasLightPattern.push_back(0b00000111);
    m_ChristmasLightPattern.push_back(0b00000110);
    m_ChristmasLightPattern.push_back(0b00000100);

    m_campfire = Graphics::FXSystem->getEffect("FireSmoke");
    m_campfire2 = Graphics::FXSystem->getEffect("ChimneySmoke");
    m_campfire3 = Graphics::FXSystem->getEffect("ChimneySmoke");


}

Map::~Map() 
{
	clear();
}

void Map::add(FrameProp frameProp)
{
  //  m_objects.push_back(new Object(frameProp.modelID, frameProp.position, frameProp.rotation));
}

void Map::add(FrameHitbox frameHitbox)
{
    //if (frameHitbox.modelID == Resources::Models::UnitCube)
    //    m_hitboxes.push_back(new StaticObject(frameHitbox.modelID, m_physicsPtr->createBody(
    //        Cube(frameHitbox.position, frameHitbox.rotation, frameHitbox.dimensions), NULL, false,
    //        Physics::COL_HITBOX,
    //        Physics::COL_EVERYTHING),
    //        {1, 1.f, 1},
    //        StaticObject::NavigationMeshFlags::CULL
    //    ));
    //else
        m_hitboxes.push_back(newd StaticObject(frameHitbox.modelID, m_physicsPtr->createBody(
            Cube(frameHitbox.position, frameHitbox.rotation, frameHitbox.dimensions), NULL, false,
            Physics::COL_HITBOX,
            Physics::COL_EVERYTHING),
            frameHitbox.dimensions,
            StaticObject::NavigationMeshFlags::NO_CULL
        ));
}

void Map::init(Physics* physics)
{
    m_physicsPtr = physics;

    // Disables debug draw as default
    m_drawDebug = false;
}

void Map::clear()
{
	for (size_t i = 0; i < m_props.size(); i++)     delete m_props[i];
	for (size_t i = 0; i < m_hitboxes.size(); i++)  delete m_hitboxes[i];
    for (size_t i = 0; i < m_lights.size(); i++)    delete m_lights[i];

	m_props.clear();
	m_hitboxes.clear();
    m_lights.clear();
}

// If user holds tab, draw debug info
void Map::update(float deltaTime)
{
    m_ChristmasLightTimer += deltaTime / 1000.f;
    if (m_ChristmasLightTimer > 1.5f) {
        m_ChristmasLightTimer = 0.f;
        m_ChristmasPatternIndex = (m_ChristmasPatternIndex + 1) % m_ChristmasLightPattern.size();
}



    //SubmitLights(m_MapLights);

#ifdef _DEBUG
    m_drawDebug = DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::LeftShift) ? true : false;
#endif // _DEBUG

    Graphics::FXSystem->processEffect(&m_campfire, { 63.604431f, 1.706378f, 29.647547f }, deltaTime / 1500.f);
    Graphics::FXSystem->processEffect(&m_campfire2, { -18.386185f, 28.395212f, -37.384571f }, deltaTime / 2000.f);
    Graphics::FXSystem->processEffect(&m_campfire3, { 100.62f, 60.83f, -85.596f }, deltaTime / 2000.f);


}

void Map::render() const
{
    auto pattern = m_ChristmasLightPattern[m_ChristmasPatternIndex];

    if (pattern & 0x1)
        SubmitLights(m_RedBulbs);
    if (pattern & 0x2)
        SubmitLights(m_GreenBulbs);
    if (pattern & 0x4)
        SubmitLights(m_BlueBulbs);

    SubmitLights(m_MapLights);

    for (StaticObject* o : m_props)     o->render();
    for (LightObject* l : m_lights)     l->render();
    for (StaticObject* e : m_hitboxes)  e->render(); // Hitboxes should not be visiable at all at release

    for (auto & d : decorations) d.render();
	for (auto & f : foliages) f.render();
}
	
std::vector<StaticObject*>*			Map::getProps()				{ return &m_props;				}
std::vector<StaticObject*>*			Map::getHitboxes()			{ return &m_hitboxes;			}
std::vector<LightObject*>*			Map::getLights()            { return &m_lights;             }

void Map::loadStartMenuScene()
{
    // Temp campfire map, remove this when an actual campfire is done
    std::vector<FrameHitbox> hitboxes;
    std::vector<FrameLight> lights;

    //hitboxes.push_back({ { 0, 0.0f, 0 },{ 0, 0, 0 },{ 1.f, 1.f, 1.f },    Resources::Models::MenuScene });

    add(FrameLight({ 0.f, 0.f, 0.f }, {1.f, 0.5f, 0.3f}, 1.f, 10.f));
    
    Sound::SoundSource campfire;
    campfire.pos = { 0, 0, 0 };
    campfire.vel = { 0, 0, 0 };
    campfire.playSFX(Sound::SFX::CAMPFIRE);

    for (size_t i = hitboxes.size(); i--;) add(hitboxes[i]);

    SpecialEffectRenderInfo info = {};
    info.type = SpecialEffectRenderInfo::Snow;
    info.restart = true;
    QueueRender(info);
}

// this method is very bad
void Map::loadMap(Resources::Maps::Files map)
{
    toml::ParseResult mapFile = toml::parseFile(Resources::Maps::Paths.at(map));
    if (!mapFile.valid()) throw std::runtime_error(mapFile.errorReason);

    toml::Value mapValue = mapFile.value;

    struct Instance
    {
        std::string name;
        std::string model;
        btVector3 translation = { 0, 0, 0 };
        btQuaternion rotation = { 0, 0, 0, 1 };
        btVector3 scale = { 1, 1, 1 };
    };

    std::vector<Instance> staticInstances;
    std::vector<Instance> foliageInstances;
    std::vector<Instance> triggerInstances;

    auto mapStatic = mapValue.find("Static");
    auto mapFoliage = mapValue.find("Foliage");
    auto mapTrigger = mapValue.find("Trigger");

    static auto pushInstances = [](toml::Value * src, std::vector<Instance> & dest)
    {
        auto vec3 = [](toml::Value const* v) -> btVector3
        {
            return
            {
                (btScalar)v->find(0)->asNumber(),
                (btScalar)v->find(1)->asNumber(),
                (btScalar)v->find(2)->asNumber(),
            };
        };

        auto quat = [](toml::Value const* v) -> btQuaternion
        {
            return
            {
                (btScalar)v->find(0)->asNumber(),
                (btScalar)v->find(1)->asNumber(),
                (btScalar)v->find(2)->asNumber(),
                (btScalar)v->find(3)->asNumber(),
            };
        };

        for (auto & tInstance : src->as<toml::Array>())
        {
            Instance instance;
            instance.name = tInstance.get<std::string>("name");
            instance.model = tInstance.get<std::string>("model");

            toml::Value const* translationValue = tInstance.findChild("translation");
            toml::Value const* rotationValue = tInstance.findChild("rotation");
            toml::Value const* scaleValue = tInstance.findChild("scale");

            if (translationValue) { instance.translation = vec3(translationValue); }
            if (rotationValue) { instance.rotation = quat(rotationValue); }
            if (scaleValue) { instance.scale = vec3(scaleValue); }

            dest.push_back(instance);
        }
    };

    if (mapStatic) pushInstances(mapStatic, staticInstances);
    if (mapFoliage) pushInstances(mapFoliage, foliageInstances);
    if (mapTrigger) pushInstances(mapTrigger, triggerInstances);
    // TODO USE THIS //    

    decorations.clear();
	foliages.clear();

    auto toVec3 = [](DirectX::SimpleMath::Vector3 & vec) -> btVector3
    {
        return {vec.x, vec.y, vec.z};
    };

    auto toQuat = [](DirectX::SimpleMath::Quaternion & vec) -> btQuaternion
    {
        return {vec.x, vec.y, vec.z, vec.w};
    };

    // this is a temporary solution
    bool groundModel = false, aiCollBox = false;
    int aiBoxCounter = 0;

    for (auto & instance : staticInstances)
    {
        try
        {
            Resources::Models::Files model = Resources::Models::toEnum(instance.model.c_str());
            groundModel = model == Resources::Models::Ground;

            DirectX::SimpleMath::Quaternion rotation(instance.rotation[0], instance.rotation[1], instance.rotation[2], instance.rotation[3]);
            DirectX::SimpleMath::Vector3 scale(instance.scale[0], instance.scale[1], instance.scale[2]);
            DirectX::SimpleMath::Vector3 translation(instance.translation[0], instance.translation[1], instance.translation[2]);

            DirectX::SimpleMath::Matrix instance_transform = DirectX::XMMatrixAffineTransformation(scale, {}, rotation, translation);
            HybrisLoader::Model::BoundingBox bounding_box = ModelLoader::get().getModel(model)->get_bounding_box();

            bounding_box.apply_scale(scale);
            Decoration decor(model, instance_transform, bounding_box.sphere_radius());
            decorations.push_back(decor);

            aiBoxCounter = 0;
            for (auto & hitbox : *ModelLoader::get().getModel(model)->getHitboxes())
            {
                if (groundModel)
                {
                    aiBoxCounter++;
                    if (aiBoxCounter > AI_BOX_ID_MIN) aiCollBox = true;
                }

                btRigidBody * body = m_physicsPtr->createHitbox(
                    toVec3(hitbox.position),
                    toQuat(hitbox.rotation),
                    toVec3(hitbox.halfSize) * instance.scale,
                    false,
                    Physics::COL_HITBOX,
                    aiCollBox ? Physics::COL_NOTHING : Physics::COL_EVERYTHING
                );
                aiCollBox = false;
                body->setUserPointer(*m_mapObject);

                float t[16]; body->getWorldTransform().getOpenGLMatrix(t);
                DirectX::SimpleMath::Matrix hitbox_transform(t);

                hitbox_transform = hitbox_transform * instance_transform;
                DirectX::SimpleMath::Quaternion rot;
                DirectX::SimpleMath::Vector3 pos, scale;
                hitbox_transform.Decompose(scale, rot, pos);
                btTransform new_transform(toQuat(rot), {pos.x, pos.y, pos.z});

                body->setWorldTransform(new_transform);
            }
        }
        catch (const char * e)
        {
            std::cerr << "Could not find model " << instance.model << " during map load. Ignoring model." << std::endl;
        }
    }
	for (auto & instance : foliageInstances)
	{
		 try
         {
			 Resources::Models::Files model = Resources::Models::toEnum(instance.model.c_str());

			 DirectX::SimpleMath::Quaternion rotation(instance.rotation[0], instance.rotation[1], instance.rotation[2], instance.rotation[3]);
			 DirectX::SimpleMath::Vector3 scale(instance.scale[0], instance.scale[1], instance.scale[2]);
			 DirectX::SimpleMath::Vector3 translation(instance.translation[0], instance.translation[1], instance.translation[2]);

			 DirectX::SimpleMath::Matrix instance_transform = DirectX::XMMatrixAffineTransformation(scale, {}, rotation, translation);
			 HybrisLoader::Model::BoundingBox bounding_box = ModelLoader::get().getModel(model)->get_bounding_box();

			 bounding_box.apply_scale(scale);
			 Foliage decor(model, instance_transform, bounding_box.sphere_radius());
			 foliages.push_back(decor);
		 }
		 catch (const char * e)
		 {
			 std::cerr << "Could not find model " << instance.model << " during map load. Ignoring model." << std::endl;
		 }
	}
    SpecialEffectRenderInfo info;
    info.type = SpecialEffectRenderInfo::Snow;
    info.restart = true;
    QueueRender(info);
}

// Adds a pointlight to the map
void Map::add(FrameLight frameLight)
{
    m_lights.push_back(newd LightObject(frameLight));
}

//
//// Adds a static hitbox to the map
//void Map::add(FrameHitbox frameHitbox)
//{
//    m_hitboxes.push_back(newd StaticObject(
//        frameHitbox.modelID, 
//        m_physicsPtr->createBody(
//            Cube(frameHitbox.position, frameHitbox.rotation, frameHitbox.dimensions), /* Shape */             
//            NULL,                   /* Mass */              
//            false,                  /* Sensor */            
//            Physics::COL_HITBOX,    /* Collision Type */    
//            Physics::COL_EVERYTHING /* Collides With */     
//        ),
//        frameHitbox.dimensions, /* Graphical Scaling */ 
//        StaticObject::NavigationMeshFlags::NO_CULL
//    ));
//}