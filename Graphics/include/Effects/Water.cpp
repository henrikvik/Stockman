//#include "Water.h"
//
//using namespace DirectX::SimpleMath;
//
//Water::Water(ID3D11Device * device) : waterShader(device, SHADER_PATH("WaterShader.hlsl"), VERTEX_DESC)
//{
//	ThrowIfFailed(DirectX::CreateDDSTextureFromFile(device, TEXTURE_PATH("waterMask.dds"), nullptr, &textureMask));
//	ThrowIfFailed(DirectX::CreateDDSTextureFromFile(device, TEXTURE_PATH("waterGradient.dds"), nullptr, &textureGradient));
//	ThrowIfFailed(DirectX::CreateDDSTextureFromFile(device, TEXTURE_PATH("waterNormal.dds"), nullptr, &textureNormal));
//}
//
//Water::~Water()
//{
//	SAFE_RELEASE(textureMask);
//	SAFE_RELEASE(textureGradient);
//	SAFE_RELEASE(textureNormal);
//}
//
//void Water::draw()
//{
//	
//	time++;
//	timeBuffer.write(deviceContext, &time, sizeof(time));
//
//	ID3D11Buffer *cameraBuffer = camera->getBuffer();
//	deviceContext->PSSetConstantBuffers(0, 1, &cameraBuffer);
//	deviceContext->VSSetConstantBuffers(0, 1, &cameraBuffer);
//
//	deviceContext->VSSetConstantBuffers(1, 1, timeBuffer);
//	deviceContext->RSSetState(states->CullNone());
//	deviceContext->OMSetRenderTargets(1, fakeBackBuffer, depthStencil);
//
//	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
//	UINT sampleMask = 0xffffffff;
//	deviceContext->OMSetBlendState(transparencyBlendState, blendFactor, sampleMask);
//
//	deviceContext->IASetInputLayout(waterShader);
//	deviceContext->VSSetShader(waterShader, nullptr, 0);
//	deviceContext->PSSetShader(waterShader, nullptr, 0);
//
//	for (WaterRenderInfo * info : renderWaterQueue)
//	{
//		ModelInfo model = resourceManager.getModelInfo(info.meshId);
//
//		static UINT stride = sizeof(Vertex), offset = 0;
//		deviceContext->IASetVertexBuffers(0, 1, &model.vertexBuffer, &stride, &offset);
//		deviceContext->IASetIndexBuffer(model.indexBuffer, DXGI_FORMAT_R32_UINT, 0);
//
//		static ID3D11ShaderResourceView * modelTextures[1] = { nullptr };
//		modelTextures[0] = model.diffuseMap;
//		deviceContext->PSSetShaderResources(0, 1, modelTextures);
//
//		deviceContext->DrawIndexed((UINT)model.indexCount, 0, 0);
//	}
//
//	renderWaterQueue.clear();
//
//}
