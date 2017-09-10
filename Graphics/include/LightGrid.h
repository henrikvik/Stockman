#pragma once

#include "Camera.h"
#include <d3d11.h>
#include <SimpleMath.h>
#include "CommonStates.h"
#include "ThrowIfFailed.h"
#include "ShaderHandler.h"
#include "Structs.h"

#define BLOCK_SIZE 16

namespace Graphics {

struct Plane {
	DirectX::SimpleMath::Vector4 pd;
};

struct Frustum {
	Plane planes[4];
};

struct DispatchParams {
	int32_t numThreadGroups[4];
	int32_t numThreads[4];
};

enum class CpuAccess {
	None = 0,
	Read = 1 << 0,
	Write = 1 << 1,
	ReadWrite = Read | Write
};

template<typename T>
class StructuredBuffer {
public:
	StructuredBuffer(ID3D11Device *device, CpuAccess access, size_t count, T* ptr = nullptr)
	{
		D3D11_BUFFER_DESC desc = {};
		desc.ByteWidth = sizeof(T) * count;
		
		if ((int)access & (int)CpuAccess::Read) {
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
			desc.Usage = D3D11_USAGE_STAGING;
		}
		else if ((int)access & (int)CpuAccess::Write) {
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			desc.Usage = D3D11_USAGE_DYNAMIC;
		}
		else {
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		}

		desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		desc.StructureByteStride = sizeof(T);

		if (ptr) {
			D3D11_SUBRESOURCE_DATA data = {};
			data.pSysMem = ptr;

			ThrowIfFailed(device->CreateBuffer(&desc, &data, &m_Buffer));
		}
		else {
			ThrowIfFailed(device->CreateBuffer(&desc, nullptr, &m_Buffer));
		}

		if (desc.BindFlags & D3D11_BIND_SHADER_RESOURCE) {
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			srvDesc.Format = DXGI_FORMAT_UNKNOWN;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
			srvDesc.Buffer.NumElements = count;

			ThrowIfFailed(device->CreateShaderResourceView(m_Buffer, &srvDesc, &m_SRV));
		}

		if (desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS) {
			D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
			uavDesc.Format = DXGI_FORMAT_UNKNOWN;
			uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
			uavDesc.Buffer.NumElements = count;

			ThrowIfFailed(device->CreateUnorderedAccessView(m_Buffer, &uavDesc, &m_UAV));
		}
	}
	~StructuredBuffer() {
		if (m_Buffer) m_Buffer->Release();
		if (m_UAV)    m_UAV->Release();
		if (m_SRV)    m_SRV->Release();
	}

	ID3D11Buffer *getBuffer() const { return m_Buffer; }
	ID3D11ShaderResourceView *getSRV() const { return m_SRV; }
	ID3D11UnorderedAccessView *getUAV() const { return m_UAV; }

	void CopyTo(ID3D11DeviceContext *cxt, StructuredBuffer<T> *other) {
		cxt->CopyResource(other->getBuffer(), this->getBuffer());
	}

	T* map(ID3D11DeviceContext *cxt) {
		D3D11_MAPPED_SUBRESOURCE data = {};
		cxt->Map(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);

		return static_cast<T*>(data.pData);
	}

	void unmap(ID3D11DeviceContext *cxt) {
		cxt->Unmap(m_Buffer, 0);
	}
private:
	ID3D11Buffer              *m_Buffer;
	ID3D11ShaderResourceView  *m_SRV;
	ID3D11UnorderedAccessView *m_UAV;
};

class LightGrid {
public:
	LightGrid();
	virtual ~LightGrid();

	void initialize(Camera *camera, ID3D11Device *device, ID3D11DeviceContext *cxt, ShaderHandler *shaders);
	void cull(Camera *camera, DirectX::CommonStates *states, ID3D11ShaderResourceView *depth, ID3D11Device *device, ID3D11DeviceContext *cxt, ShaderHandler *shaders);

	StructuredBuffer<uint32_t> *getOpaqueIndexCounter() const {	return m_OpaqueIndexCounter; }
	StructuredBuffer<uint32_t> *getTransparentIndexCounter() const { return m_TransparentIndexCounter; }

	StructuredBuffer<uint32_t> *getOpaqueIndexList() const { return m_OpaqueIndexList; }
	StructuredBuffer<uint32_t> *getTransparentIndexList() const { return m_TransparentIndexList; }

	StructuredBuffer<Frustum> *getFrustums() const { return m_Frustums; }
	StructuredBuffer<Light> *getLights() const { return m_Lights; }

	// TEMP:
	ID3D11ShaderResourceView *getOpaqueLightGridSRV() const { return m_OpaqueLightGridSRV; }
	ID3D11ShaderResourceView *getTransparentLightGridSRV() const { return m_TransparentLightGridSRV; }
	ID3D11ShaderResourceView *getDebugSRV() const { return m_DebugSRV; }
private:
	void generateFrustums(Camera *camera, ID3D11Device *device, ID3D11DeviceContext *cxt, ShaderHandler *shaders);

	DispatchParams m_Params;
	ID3D11Buffer  *m_ParamsBuffer;
	ID3D11Buffer  *m_Frustrums;

	int m_FrustumGenerationCS;
	int m_CullingCS;

	StructuredBuffer<uint32_t> *m_ResetIndexCounter;
	StructuredBuffer<uint32_t> *m_OpaqueIndexCounter;
	StructuredBuffer<uint32_t> *m_TransparentIndexCounter;

	StructuredBuffer<uint32_t> *m_OpaqueIndexList;
	StructuredBuffer<uint32_t> *m_TransparentIndexList;

	StructuredBuffer<Frustum>  *m_Frustums;
	StructuredBuffer<Light>    *m_Lights;


	ID3D11UnorderedAccessView *m_DebugUAV;
	ID3D11ShaderResourceView  *m_DebugSRV;

	ID3D11Buffer              *m_ResetIndexCounterBuffer;
	ID3D11UnorderedAccessView *m_ResetIndexCounterUAV;
	ID3D11ShaderResourceView  *m_ResetIndexCounterSRV;

	ID3D11Buffer              *m_OpaqueIndexCounterBuffer;
	ID3D11UnorderedAccessView *m_OpaqueIndexCounterUAV;
	ID3D11ShaderResourceView  *m_OpaqueIndexCounterSRV;

	ID3D11Buffer              *m_TransparentIndexCounterBuffer;
	ID3D11UnorderedAccessView *m_TransparentIndexCounterUAV;
	ID3D11ShaderResourceView  *m_TransparentIndexCounterSRV;

	ID3D11UnorderedAccessView *m_OpaqueIndexListUAV;
	ID3D11ShaderResourceView  *m_OpaqueIndexListSRV;
	ID3D11UnorderedAccessView *m_TransparentIndexListUAV;
	ID3D11ShaderResourceView  *m_TransparentIndexListSRV;

	ID3D11UnorderedAccessView *m_OpaqueLightGridUAV;
	ID3D11ShaderResourceView  *m_OpaqueLightGridSRV;
	ID3D11UnorderedAccessView *m_TransparentLightGridUAV;
	ID3D11ShaderResourceView  *m_TransparentLightGridSRV;

	ID3D11Buffer              *mm_Lights;
	ID3D11UnorderedAccessView *m_LightsUAV;
	ID3D11ShaderResourceView  *m_LightsSRV;

	ID3D11UnorderedAccessView *m_FrustrumsUAV;
	ID3D11ShaderResourceView  *m_FrustrumsSRV;

	ID3D11ShaderResourceView  *gradientSRV;
};

}