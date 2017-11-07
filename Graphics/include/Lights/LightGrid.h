#pragma once

#include <d3d11.h>
#include <SimpleMath.h>
#include <CommonStates.h>
#include "../Camera.h"
#include "../ThrowIfFailed.h"
#include "../Structs.h"
//#include "../Resources/ResourceManager.h"
#include "../Utility/Shader.h"
#include "../Utility/StructuredBuffer.h"

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

	class LightGrid {
	public:
		LightGrid();
		virtual ~LightGrid();

		void initialize();
		void cull(
            ID3D11Buffer * cameraBuffer, 
            ID3D11ShaderResourceView *depth, 
            ID3D11ShaderResourceView * lights, 
            ID3D11UnorderedAccessView * lightOpaqueGrid,
            ID3D11UnorderedAccessView * lightOpaqueIndexList
        ) const;

		StructuredBuffer<uint32_t> *getOpaqueIndexCounter() const { return m_OpaqueIndexCounter; }
		StructuredBuffer<uint32_t> *getTransparentIndexCounter() const { return m_TransparentIndexCounter; }

		StructuredBuffer<uint32_t> *getTransparentIndexList() const { return m_TransparentIndexList; }

		StructuredBuffer<Frustum> *getFrustums() const { return m_Frustums; }

		// TEMP:
		ID3D11ShaderResourceView *getTransparentLightGridSRV() const { return m_TransparentLightGridSRV; }
		ID3D11ShaderResourceView *getDebugSRV() const { return m_DebugSRV; }
	private:
		void generateFrustumsCPU(Camera *camera, ID3D11Device *device);
		void generateFrustums(Camera *camera, ID3D11Device *device, ID3D11DeviceContext *cxt);

		DispatchParams m_Params;
		ID3D11Buffer  *m_ParamsBuffer;

		StructuredBuffer<uint32_t> *m_ResetIndexCounter;
		StructuredBuffer<uint32_t> *m_OpaqueIndexCounter;
		StructuredBuffer<uint32_t> *m_TransparentIndexCounter;

		StructuredBuffer<uint32_t> *m_TransparentIndexList;

		StructuredBuffer<Frustum>  *m_Frustums;

		ID3D11UnorderedAccessView *m_DebugUAV;
		ID3D11ShaderResourceView  *m_DebugSRV;

		ID3D11UnorderedAccessView *m_TransparentLightGridUAV;
		ID3D11ShaderResourceView  *m_TransparentLightGridSRV;

		ID3D11ShaderResourceView  *gradientSRV;

		ComputeShader *m_CullGrids;
		ComputeShader *m_FrustumGeneration;
	};

}