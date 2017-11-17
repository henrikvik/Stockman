#include "LightGrid.h"
#include <WICTextureLoader.h>
#include <Engine\Constants.h>
#include "../Device.h"
#include "../CommonState.h"
#include "../MainCamera.h"

namespace Graphics {

	LightGrid::LightGrid()
	{
	}

	LightGrid::~LightGrid()
	{
		delete m_ResetIndexCounter;
		delete m_OpaqueIndexCounter;
		delete m_TransparentIndexCounter;
		delete m_TransparentIndexList;
		delete m_Frustums;
		delete m_CullGrids;
		delete m_FrustumGeneration;

		SAFE_RELEASE(m_ParamsBuffer);

		SAFE_RELEASE(m_DebugUAV);
		SAFE_RELEASE(m_DebugSRV);

		SAFE_RELEASE(m_TransparentLightGridUAV);
		SAFE_RELEASE(m_TransparentLightGridSRV);

		SAFE_RELEASE(gradientSRV);
	}

	void LightGrid::initialize()
	{
		generateFrustumsCPU(Global::mainCamera, Global::device);
		//

		//generateFrustums(camera, device, cxt, shaders);

		m_CullGrids = newd ComputeShader(Resources::Shaders::LightGridCulling);
        m_FrustumGeneration = nullptr;// newd ComputeShader(Global::device, SHADER_PATH("LightGridGeneration.hlsl"));
		
		uint32_t initial = 0;
		m_ResetIndexCounter = newd StructuredBuffer<uint32_t>(Global::device, CpuAccess::Read, 1, &initial);
		m_OpaqueIndexCounter = newd StructuredBuffer<uint32_t>(Global::device, CpuAccess::None, 1, &initial);
		m_TransparentIndexCounter = newd StructuredBuffer<uint32_t>(Global::device, CpuAccess::None, 1, &initial);

		m_TransparentIndexList = newd StructuredBuffer<uint32_t>(Global::device, CpuAccess::None, INDEX_LIST_SIZE);


#pragma region Light Grid UAVs
		{
			D3D11_TEXTURE2D_DESC desc = {};
			desc.Format = DXGI_FORMAT_R32G32_UINT;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
			desc.Width = m_Params.numThreadGroups[0];
			desc.Height = m_Params.numThreadGroups[1];
			desc.SampleDesc.Count = 1;
			desc.MipLevels = 1;
			desc.ArraySize = 1;

			D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
			ZeroMemory(&uavDesc, sizeof(uavDesc));
			uavDesc.Format = DXGI_FORMAT_R32G32_UINT;
			uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;

			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			ZeroMemory(&srvDesc, sizeof(srvDesc));
			srvDesc.Format = DXGI_FORMAT_R32G32_UINT;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = 1;


			ID3D11Texture2D *texture;
			ThrowIfFailed(Global::device->CreateTexture2D(&desc, nullptr, &texture));
			ThrowIfFailed(Global::device->CreateUnorderedAccessView(texture, &uavDesc, &m_TransparentLightGridUAV));
			ThrowIfFailed(Global::device->CreateShaderResourceView(texture, &srvDesc, &m_TransparentLightGridSRV));
			SAFE_RELEASE(texture);
		}

#pragma endregion

#pragma region Debug heatmap

		ID3D11Texture2D *texture;

		ThrowIfFailed(DirectX::CreateWICTextureFromFile(Global::device, TEXTURE_PATH("heatmap.png"), nullptr, &gradientSRV));

		{
			D3D11_TEXTURE2D_DESC desc = {};
			desc.Width = 1280;
			desc.Height = 720;
			desc.SampleDesc.Count = 1;
			desc.ArraySize = 1;

			desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;

			ThrowIfFailed(Global::device->CreateTexture2D(&desc, nullptr, &texture));
		}

		{
			D3D11_UNORDERED_ACCESS_VIEW_DESC desc = {};
			desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;

			ThrowIfFailed(Global::device->CreateUnorderedAccessView(texture, &desc, &m_DebugUAV));
		}

		{
			D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
			desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipLevels = 1;

			ThrowIfFailed(Global::device->CreateShaderResourceView(texture, &desc, &m_DebugSRV));
		}

		SAFE_RELEASE(texture);
#pragma endregion
	}

	void LightGrid::cull(
        ID3D11Buffer * cameraBuffer, 
        ID3D11ShaderResourceView *depth,
        ID3D11ShaderResourceView * lights,
        ID3D11UnorderedAccessView * lightOpaqueGrid,
        ID3D11UnorderedAccessView * lightOpaqueIndexList
    ) const
	{
		//generateFrustums(camera, device, cxt, shaders);
		m_ResetIndexCounter->CopyTo(Global::context, m_OpaqueIndexCounter);
		m_ResetIndexCounter->CopyTo(Global::context, m_TransparentIndexCounter);

		ID3D11SamplerState *sampler = Global::cStates->LinearClamp();
		ID3D11Buffer *bufs[] = {
            cameraBuffer,
			m_ParamsBuffer
		};
		ID3D11ShaderResourceView *SRVs[] = {
			depth,
			m_Frustums->getSRV(),
            lights,
			gradientSRV
		};
		ID3D11UnorderedAccessView *UAVs[] = {
			m_OpaqueIndexCounter->getUAV(),
			m_TransparentIndexCounter->getUAV(),
            lightOpaqueIndexList,
			m_TransparentIndexList->getUAV(),
            lightOpaqueGrid,
			m_TransparentLightGridUAV,
			m_DebugUAV
		};

		Global::context->CSSetShader(*m_CullGrids, nullptr, 0);


		Global::context->CSSetSamplers(0, 1, &sampler);
		Global::context->CSSetConstantBuffers(0, 2, bufs);
		Global::context->CSSetShaderResources(0, 4, SRVs);
		Global::context->CSSetUnorderedAccessViews(0, 7, UAVs, nullptr);

        Global::context->Dispatch(
			m_Params.numThreadGroups[0],
			m_Params.numThreadGroups[1],
			1
		);

		Global::context->CSSetShaderResources(0, 4, Global::nulls);
		Global::context->CSSetUnorderedAccessViews(0, 7, Global::nulls, nullptr);
		Global::context->CSSetShader(nullptr, nullptr, 0);
	}

	inline Plane computePlane(DirectX::SimpleMath::Vector3 a, DirectX::SimpleMath::Vector3 b)
	{
		Plane plane;

		auto v = a.Cross(b);
		v.Normalize();

		plane.pd = DirectX::SimpleMath::Vector4(v);
		plane.pd.w = 0;

		return plane;
	}

	void LightGrid::generateFrustumsCPU(Camera * camera, ID3D11Device * device)
	{
		using namespace DirectX::SimpleMath;

		m_Params.numThreads[0] = (int)ceil(1280 / (float)BLOCK_SIZE);
		m_Params.numThreads[1] = (int)ceil(720 / (float)BLOCK_SIZE);
		m_Params.numThreads[2] = 1;

		m_Params.numThreadGroups[0] = (int)ceil(m_Params.numThreads[0] / (float)BLOCK_SIZE);
		m_Params.numThreadGroups[1] = (int)ceil(m_Params.numThreads[1] / (float)BLOCK_SIZE);
		m_Params.numThreadGroups[2] = 1;



		auto count = m_Params.numThreads[0] * m_Params.numThreads[1];
		auto frustums = new Frustum[count];

		auto invProj = camera->getProj().Invert();

		// thread groups
		for (auto groupY = 0; groupY < m_Params.numThreadGroups[1]; groupY++) {
			for (auto groupX = 0; groupX < m_Params.numThreadGroups[0]; groupX++) {

				// group threads
				for (auto threadY = 0; threadY < BLOCK_SIZE; threadY++) {
					for (auto threadX = 0; threadX < BLOCK_SIZE; threadX++) {

						auto threadIdX = (groupX * BLOCK_SIZE + threadX);
						auto threadIdY = (groupY * BLOCK_SIZE + threadY);

						Vector4 corners[] = {
							Vector4(float(threadIdX       * BLOCK_SIZE),  float(threadIdY       * BLOCK_SIZE), 1.f, 1.f),
							Vector4(float((threadIdX + 1) * BLOCK_SIZE),  float(threadIdY       * BLOCK_SIZE), 1.f, 1.f),
							Vector4(float(threadIdX       * BLOCK_SIZE),  float((threadIdY + 1) * BLOCK_SIZE), 1.f, 1.f),
							Vector4(float((threadIdX + 1) * BLOCK_SIZE),  float((threadIdY + 1) * BLOCK_SIZE), 1.f, 1.f)
						};

						Vector3 points[4];
						for (auto i = 0; i < 4; i++) {
							auto vec = corners[i];
							auto coord = Vector2(vec.x, vec.y) / Vector2(1280, 720);
							auto clip = Vector4(coord.x * 2.f - 1.f, (1.f - coord.y) * 2.f - 1.f, vec.z, vec.w);
							auto view = Vector4(DirectX::XMVector4Transform(clip, invProj));
							view = view / view.w;

							points[i] = Vector3(view);
						}

						Frustum frustum;
						frustum.planes[0] = computePlane(points[2], points[0]);
						frustum.planes[1] = computePlane(points[1], points[3]);
						frustum.planes[2] = computePlane(points[0], points[1]);
						frustum.planes[3] = computePlane(points[3], points[2]);

						if (threadIdX < m_Params.numThreads[0] && threadIdY < m_Params.numThreads[1]) {
							auto idx = threadIdX + (threadIdY * m_Params.numThreads[0]);
							frustums[idx] = frustum;
						}
					}
				}
			}
		}

		m_Frustums = new StructuredBuffer<Frustum>(device, CpuAccess::None, count, frustums);
		delete[] frustums;


		m_Params.numThreadGroups[0] = (int)ceil(1280 / (float)BLOCK_SIZE);
		m_Params.numThreadGroups[1] = (int)ceil(720 / (float)BLOCK_SIZE);
		m_Params.numThreadGroups[2] = 1;

		m_Params.numThreads[0] = (int)m_Params.numThreadGroups[0] * BLOCK_SIZE;
		m_Params.numThreads[1] = (int)m_Params.numThreadGroups[1] * BLOCK_SIZE;
		m_Params.numThreads[2] = 1;

		// Grid params
		{
			D3D11_BUFFER_DESC desc = {};
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.ByteWidth = (UINT)sizeof(DispatchParams);
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.Usage = D3D11_USAGE_DYNAMIC;

			D3D11_SUBRESOURCE_DATA data = {};
			data.pSysMem = &m_Params;

			ThrowIfFailed(device->CreateBuffer(&desc, &data, &m_ParamsBuffer));
		}
	}

	void LightGrid::generateFrustums(Camera *camera, ID3D11Device *device, ID3D11DeviceContext *cxt)
	{
		m_Params.numThreads[0] = (int)ceil(1280 / (float)BLOCK_SIZE);
		m_Params.numThreads[1] = (int)ceil(720 / (float)BLOCK_SIZE);
		m_Params.numThreads[2] = 1;

		m_Params.numThreadGroups[0] = (int)ceil(m_Params.numThreads[0] / (float)BLOCK_SIZE);
		m_Params.numThreadGroups[1] = (int)ceil(m_Params.numThreads[1] / (float)BLOCK_SIZE);
		m_Params.numThreadGroups[2] = 1;


		// Grid params
		{
			D3D11_BUFFER_DESC desc = {};
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.ByteWidth = (UINT)sizeof(DispatchParams);
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.Usage = D3D11_USAGE_DYNAMIC;

			D3D11_SUBRESOURCE_DATA data = {};
			data.pSysMem = &m_Params;

			ThrowIfFailed(device->CreateBuffer(&desc, &data, &m_ParamsBuffer));
		}

		// Frustums
		auto count = m_Params.numThreads[0] * m_Params.numThreads[1];
		m_Frustums = newd StructuredBuffer<Frustum>(device, CpuAccess::None, count);

		cxt->CSSetShader(*m_FrustumGeneration, nullptr, 0);

		ID3D11Buffer *buffers[] = {
			*camera->getBuffer(),
			m_ParamsBuffer,
		};
		cxt->CSSetConstantBuffers(0, 2, buffers);

		auto uav = m_Frustums->getUAV();
		cxt->CSSetUnorderedAccessViews(0, 1, &uav, 0);

		cxt->Dispatch(m_Params.numThreadGroups[0], m_Params.numThreadGroups[1], 1);
		ID3D11UnorderedAccessView *reset = nullptr;
		cxt->CSSetUnorderedAccessViews(0, 1, &reset, 0);


		m_Params.numThreadGroups[0] = (int)ceil(1280 / (float)BLOCK_SIZE);
		m_Params.numThreadGroups[1] = (int)ceil(720 / (float)BLOCK_SIZE);
		m_Params.numThreadGroups[2] = 1;

		m_Params.numThreads[0] = (int)m_Params.numThreadGroups[0] * BLOCK_SIZE;
		m_Params.numThreads[1] = (int)m_Params.numThreadGroups[1] * BLOCK_SIZE;
		m_Params.numThreads[2] = 1;


		D3D11_MAPPED_SUBRESOURCE mapped_data;
		cxt->Map(m_ParamsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_data);
		CopyMemory(mapped_data.pData, &m_Params, sizeof(DispatchParams));
		cxt->Unmap(m_ParamsBuffer, 0);
	}

}