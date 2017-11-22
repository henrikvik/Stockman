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

#define INDEX_LIST_SIZE ((int)ceil(WIN_WIDTH / (float)BLOCK_SIZE) * (int)ceil(WIN_HEIGHT / (float)BLOCK_SIZE) * AVG_TILE_LIGHTS)

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

		StructuredBuffer<Frustum> *getFrustums() const { return m_Frustums; }

	private:
		void generateFrustumsCPU(Camera *camera, ID3D11Device *device);

		DispatchParams m_Params;
		ID3D11Buffer  *m_ParamsBuffer;

		StructuredBuffer<uint32_t> *m_ResetIndexCounter;
		StructuredBuffer<uint32_t> *m_OpaqueIndexCounter;

		StructuredBuffer<Frustum>  *m_Frustums;

		ComputeShader *m_CullGrids;
	};

}