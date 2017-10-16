#pragma once

#include <vector>
#include <d3d11.h>

#include <SimpleMath.h>
#include <CommonStates.h>

#include "Particle.h"
#include "../Camera.h"
#include "../Utility/Buffer.h"

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

	void render(Camera *cam, DirectX::CommonStates *states, ID3D11DepthStencilView *dest_dsv, bool debug);
	void update(float dt);
private:
	struct SphereVertex {
		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT2 uv;
	};

	struct GeometryParticleInstance {
		XMMATRIX m_Model;
		XMVECTOR m_Color;
		float m_Age;
		float m_Deform;
		float m_DeformSpeed;

		float m_NoiseScale;
		float m_NoiseSpeed;
	};

	// runtime
	std::vector<ParticleEffectInstance> m_ParticleEffects;
	std::vector<GeometryParticle> m_GeometryParticles;

	// graphics
	Buffer<SphereVertex> m_SphereVertexBuffer;
	Buffer<UINT16> m_SphereIndexBuffer;
	Buffer<GeometryParticleInstance> m_GeometryInstanceBuffer;

	ID3D11InputLayout *m_GeometryLayout;
	ID3D11VertexShader *m_GeometryVS;

	// constants
	std::vector<ParticleEffect> m_EffectDefinitions;
	std::vector<ID3D11PixelShader*> m_Materials;
	std::vector<ID3D11ShaderResourceView*> m_Textures;
};

}