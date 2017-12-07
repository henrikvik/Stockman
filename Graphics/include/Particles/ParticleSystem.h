#pragma once

#include <vector>
#include <d3d11.h>

#include <SimpleMath.h>

#include "Particle.h"
#include "../Camera.h"
#include "../Utility/Buffer.h"
#include "../Utility/Shader.h"
#include "../../export.h"

namespace Graphics {;

using namespace DirectX;

// runtime representation of a particle, all properties regarding visuals are
// deferred to the "definition", which most have a start/end value which is
// lerped between using the particles age/lifetime.
struct GeometryParticle {
    XMFLOAT3 pos;
    XMFLOAT3 velocity;
    float rotvel;
    float rotprog;
    XMFLOAT3 rot;
    GeometryDefinition *def;
    float age;
    int idx;
};

// runtime representation of a managed particle effect/emitter. this is
// entirely handled by the particle system.
struct ParticleEffectInstance {
    XMVECTOR position;
    ParticleEffect effect;
};

// runtime representation of a "anchored" particle effect. this means that all
// particles spawned by this effect/emitter have their origin on the emitter
// itself and are all owned by the effect.
//
// NOTE: when adding a anchored particle effect to the particle system, the
//       effect must be "processed" for as long as the effect persists, and
//       eventually transfer ownership back to the particlesystem.
struct AnchoredParticleEffect {
    DirectX::XMVECTOR m_Anchor;
    std::vector<GeometryParticle> m_Children;

    ParticleEffect m_Effect;
};

// internal representation of the particle instance data.
//
// NOTE: whenever changing this the corresponding input layout should also be
//       updated.
struct GeometryParticleInstance {
    XMMATRIX m_Model;
    XMVECTOR m_Color;
    float m_Age;
    float m_Deform;
    float m_DeformSpeed;

    float m_NoiseScale;
    float m_NoiseSpeed;
};

class GRAPHICS_API ParticleSystem {
public:
    ParticleSystem(ID3D11Device *device, uint32_t capacity, const char *path);
    virtual ~ParticleSystem();

    // updates an anchored effect/emitter.
    //
    // this will spawn particles that are owned by the particle effect/emitter
    // and thus need to be "processed" every frame until the user decides to
    // transfer back ownership to the particle system (making it un-anchored).
    //
    // returns true if the particle effect is over (age > lifetime)
    bool processAnchoredEffect(AnchoredParticleEffect *fx, DirectX::SimpleMath::Vector3 pos, float dt);
    
    // updates an effect/emitter.
    //
    // this will spawn particles that are owned and managed by the particle
    // system.
    //
    // returns true if the particle effect is over (age > lifetime)
    bool processEffect(ParticleEffect *fx, DirectX::SimpleMath::Vector3 pos, float dt);

    // updates an effect/emitter.
    //
    // this will spawn particles that are owned and managed by the particle
    // system with a given start velocity.
    //
    // returns true if the particle effect is over (age > lifetime)
    bool processEffect(ParticleEffect *fx, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 velocity, float dt);

    // adds a static effect/emitter
    //
    // this will add a particle effect/emitter that is owned and managed by
    // the particle system. this means that it should usually only be called
    // once per "effect" in the game
    void addEffect(std::string name, XMVECTOR pos);

    // retrieves a particle effect/emitter from the particle system.
    //
    // searches the loaded particle effect/emitter definitions and returns a
    // copy. required when using `processEffect` and `processAnchoredEffect`.
    //
    // NOTE: particle effect/emitters contain state (age) and if they are not
    //       looped effects/emitters the user has to manage creating these
    //
    // NOTE: crashes when it cant find `name`
    ParticleEffect getEffect(std::string name);

    
    void renderPrePass(ID3D11DeviceContext *cxt, Camera *cam, ID3D11DepthStencilView *dest_dsv);
    void render(
        ID3D11DeviceContext *cxt,
        Camera *cam,
        ID3D11ShaderResourceView *lightIndexList,
        ID3D11ShaderResourceView *lightGrid,
        ID3D11ShaderResourceView *lights,
        ID3D11Buffer *lightBuffer,
        ID3D11ShaderResourceView *shadowMap,
        ID3D11RenderTargetView *dest_rtv, 
        ID3D11DepthStencilView *dest_dsv,
        bool debug);

    void update(ID3D11DeviceContext *cxt, Camera *cam, float dt);
private:
    GeometryParticleInstance *uploadParticles(std::vector<GeometryParticle> &particles, GeometryParticleInstance *output, GeometryParticleInstance *max);
    void updateParticles(XMVECTOR anchor, std::vector<GeometryParticle> &particles, float dt);

    void readParticleFile(ID3D11Device *device, const char *path);
    void readSphereModel(ID3D11Device *device);

    struct SphereVertex {
        XMFLOAT3 position;
        XMFLOAT3 normal;
        XMFLOAT2 uv;
    };

    // runtime
    uint32_t m_Capacity;

    std::vector<AnchoredParticleEffect*> m_AnchorParticleEffects;
    std::vector<ParticleEffectInstance> m_ParticleEffects;
    std::vector<GeometryParticle> m_GeometryParticles;
    std::vector<GeometryParticle> m_FrameGeometryParticles;

    // graphics
    UINT m_SphereIndices;
    Buffer<SphereVertex> *m_SphereVertexBuffer;
    Buffer<UINT16> *m_SphereIndexBuffer;
    Buffer<GeometryParticleInstance> *m_GeometryInstanceBuffer;

    Shader *m_GeometryVS;

    // constants
    std::vector<ParticleEffect> m_EffectDefinitions;
    std::vector<GeometryDefinition> m_GeometryDefinitions;
    std::vector<std::tuple<ID3D11PixelShader*, ID3D11PixelShader*>> m_Materials;
    std::vector<ID3D11ShaderResourceView*> m_Textures;
};

// global variable for particle system. constructed and destroyed by renderer.
extern GRAPHICS_API ParticleSystem *FXSystem;

}
