#include "ParticleSystem.h"

#include <fstream>

namespace Graphics {;

ParticleSystem::ParticleSystem(ID3D11Device * device, uint32_t capacity, const char * path)
{
	readSphereModel(device);

	m_GeometryParticles.reserve(512);
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

	ID3DBlob *blob = compile_shader(L"Resources/Shaders/GeometryParticle.hlsl", "VS", "vs_5_0", device);
	DXCALL(device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_DefaultGeometryVS));

	D3D11_INPUT_ELEMENT_DESC input_desc[] = {
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
	};
	m_DefaultGeometryLayout = create_input_layout(input_desc, ARRAYSIZE(input_desc), blob->GetBufferPointer(), blob->GetBufferSize(), device);


}

}
