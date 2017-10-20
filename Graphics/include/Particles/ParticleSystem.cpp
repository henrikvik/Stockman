#include "ParticleSystem.h"

#include <fstream>
#include <algorithm>

namespace Graphics {;

ParticleSystem::ParticleSystem(ID3D11Device * device, uint32_t capacity, const char * path)
{
	readSphereModel(device);

	m_GeometryParticles.reserve(512);
}

void ParticleSystem::renderPrePass(ID3D11DeviceContext * cxt, Camera * cam, DirectX::CommonStates * states, ID3D11DepthStencilView * dest_dsv)
{
}

void ParticleSystem::render(ID3D11DeviceContext *cxt, Camera * cam, DirectX::CommonStates * states, ID3D11RenderTargetView *dest_rtv, ID3D11DepthStencilView * dest_dsv, bool debug)
{
	ID3D11SamplerState *samplers[] = {
		states->LinearClamp(),
		states->LinearWrap(),
		states->PointClamp(),
		states->PointWrap()
	};
	cxt->VSSetSamplers(0, 4, samplers);
	cxt->PSSetSamplers(0, 4, samplers);

	// spheres
	{
		UINT strides[2] = {
			(UINT)sizeof(SphereVertex),
			(UINT)sizeof(GeometryParticleInstance)
		};

		UINT offsets[2] = {
			0,
			0
		};

		ID3D11Buffer *buffers[] = {
			*m_SphereVertexBuffer,
			*m_GeometryInstanceBuffer
		};

		cxt->IASetInputLayout(*m_GeometryVS);
		cxt->IASetVertexBuffers(0, 2, buffers, strides, offsets);
		cxt->IASetIndexBuffer(*m_SphereIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
		cxt->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		cxt->VSSetShader(*m_GeometryVS, nullptr, 0);
		auto buf = cam->getBuffer();
		cxt->VSSetConstantBuffers(0, 1, &buf);

		cxt->OMSetDepthStencilState(states->DepthDefault(), 0);
		cxt->OMSetRenderTargets(1, &dest_rtv, dest_dsv);
		cxt->RSSetState(states->CullNone());

		int offset = 0;
		int len = 0;
		int current_material = -1;

		for (int i = 0; i < m_GeometryParticles.size(); i++) {
			auto &particle = m_GeometryParticles[i];

			// initialize state during first loop
			if (current_material == -1) {
				current_material = particle.idx;
			}
			// if material changes, flush our built up batching
			else if (current_material != particle.idx) {
				cxt->PSSetShader(m_Materials[current_material], nullptr, 0);
				cxt->DrawIndexedInstanced(m_SphereIndices, len, 0, 0, offset);

				current_material = particle.idx;
				offset = i;
				len = 0;
			}

			len++;
		}
	}
}

void ParticleSystem::update(ID3D11DeviceContext *cxt, Camera * cam, float dt)
{
	// TODO: sort by material index

	{
		GeometryParticleInstance *ptr = m_GeometryInstanceBuffer->map(cxt);

		auto it = m_GeometryParticles.begin();
		int i = 0;
		while (it != m_GeometryParticles.end() && i < 256) {
			auto &particle = *it;
			auto def = *particle.def;

			if (particle.age > def.m_Lifetime) {
				it = m_GeometryParticles.erase(it);
				continue;
			}

			auto factor = particle.age / def.m_Lifetime;

			auto ease_color = GetEaseFuncV(def.m_ColorEasing);
			auto ease_deform = GetEaseFunc(def.m_DeformEasing);
			auto ease_size = GetEaseFunc(def.m_SizeEasing);

			auto scale = ease_size(def.m_SizeStart, def.m_SizeEnd, factor);

			ptr->m_Model = XMMatrixRotationAxis(XMLoadFloat3(&particle.rot), (particle.rotprog + particle.age) * particle.rotvel)  * XMMatrixScaling(scale, scale, scale) * XMMatrixTranslationFromVector(XMLoadFloat3(&particle.pos));
			ptr->m_Age = factor;

			auto col_start = XMFLOAT4((float*)def.m_ColorStart);
			auto col_end = XMFLOAT4((float*)def.m_ColorStart);

			ptr->m_Color = ease_color(DirectX::XMLoadFloat4(&col_start), DirectX::XMLoadFloat4(&col_end), factor);
			ptr->m_Deform = ease_deform(def.m_DeformStart, def.m_DeformEnd, factor);
			ptr->m_DeformSpeed = def.m_DeformSpeed;
			ptr->m_NoiseScale = def.m_NoiseScale;
			ptr->m_NoiseSpeed = def.m_NoiseSpeed;

			particle.age += dt;
			particle.rotprog += dt;

			XMStoreFloat3(&particle.pos, XMLoadFloat3(&particle.pos) + XMLoadFloat3(&particle.velocity) * dt);

			it++;
			ptr++;
			i++;
			// todo: remove
		}
		m_GeometryInstanceBuffer->unmap(cxt);
	}
}

void ParticleSystem::readSphereModel(ID3D11Device *device)
{
	std::ifstream meshfile("Resources/Mesh/sphere.dat", std::ifstream::in | std::ifstream::binary);
	if (!meshfile.is_open())
		throw "Can't load sky dome mesh";

	uint32_t vertexcount = 0;
	uint32_t indexcount = 0;

	meshfile.read(reinterpret_cast<char*>(&vertexcount), sizeof(uint32_t));
	if (!vertexcount)
		throw "Parse error";

	meshfile.read(reinterpret_cast<char*>(&indexcount), sizeof(uint32_t));
	if (!indexcount)
		throw "Parse error";

	std::vector<SphereVertex> vertices;
	std::vector<UINT16> indices;

	vertices.resize(vertexcount);
	meshfile.read(reinterpret_cast<char*>(&vertices.front()), sizeof(SphereVertex) * vertexcount);

	indices.resize(indexcount);
	meshfile.read(reinterpret_cast<char*>(&indices.front()), sizeof(UINT16) * indexcount);

	m_SphereIndices = indexcount;
	m_SphereVertexBuffer = new Buffer<SphereVertex>(device, BufferUsage::Immutable, BufferBind::VertexBuffer, BufferCpuAccess::None, BufferMisc::None, vertexcount, &vertices[0]);
	m_SphereIndexBuffer = new Buffer<UINT16>(device, BufferUsage::Immutable, BufferBind::IndexBuffer, BufferCpuAccess::None, BufferMisc::None, indexcount, &indices[0]);

	m_GeometryInstanceBuffer = new Buffer<GeometryParticleInstance>(device, BufferUsage::Dynamic, BufferBind::VertexBuffer, BufferCpuAccess::Write, BufferMisc::None, 512);

	m_GeometryVS = new Shader(device, L"Resources/Shaders/GeometryParticle.hlsl", {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "MODEL",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0,                            D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "MODEL",       1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "MODEL",       2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "MODEL",       3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "AGE",         0, DXGI_FORMAT_R32_FLOAT,          1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "DEFORM",      0, DXGI_FORMAT_R32_FLOAT,          1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "DEFORMSPEED", 0, DXGI_FORMAT_R32_FLOAT,          1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "NOISESCALE",  0, DXGI_FORMAT_R32_FLOAT,          1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "NOISESPEED",  0, DXGI_FORMAT_R32_FLOAT,          1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	}, VS);
}

}
