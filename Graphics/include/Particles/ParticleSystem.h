#pragma once

#include <vector>
#include <d3d11.h>

#include <SimpleMath.h>
#include <CommonStates.h>

#include "Particle.h"
#include "../Camera.h"
#include "../Utility/Buffer.h"
#include "../Utility/Shader.h"

namespace Graphics {;

using namespace DirectX;

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


struct ParticleEffectInstance {
	XMVECTOR position;
	ParticleEffect effect;
};

class ParticleSystem {

public:
	ParticleSystem(ID3D11Device *device, uint32_t capacity, const char *path);
    virtual ~ParticleSystem();

    bool processEffect(ParticleEffect *fx, DirectX::SimpleMath::Matrix model, float dt);
    void addEffect(std::string name, XMMATRIX model);
    ParticleEffect getEffect(std::string name);

	void renderPrePass(ID3D11DeviceContext *cxt, Camera *cam, DirectX::CommonStates *states, ID3D11DepthStencilView *dest_dsv);
	void render(ID3D11DeviceContext *cxt, Camera *cam, DirectX::CommonStates *states, ID3D11RenderTargetView *dest_rtv, ID3D11DepthStencilView *dest_dsv, bool debug);
	void update(ID3D11DeviceContext *cxt, Camera *cam, float dt);

    struct GeometryParticleInstance {
        XMMATRIX m_Model;
        XMVECTOR m_Color;
        float m_Age;
        float m_Deform;
        float m_DeformSpeed;

        float m_NoiseScale;
        float m_NoiseSpeed;
    };
private:
	void readParticleFile(ID3D11Device *device, const char *path);
	void readSphereModel(ID3D11Device *device);

	struct SphereVertex {
		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT2 uv;
	};

	// runtime
	uint32_t m_Capacity;

	std::vector<ParticleEffectInstance> m_ParticleEffects;
	std::vector<GeometryParticle> m_GeometryParticles;

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

extern ParticleSystem *FXSystem;

}