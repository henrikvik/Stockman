#pragma once
#include <initializer_list>
#include <vector>
#include <d3d11.h>
#include "../RenderQueue.h"
#include "../Utility/ModelLoader.h"
#include "../CommonState.h"
#include "../Device.h"
#include "../Utility/ConstantBuffer.h"

namespace Graphics
{
    class RenderPass
    {
    public:
        RenderPass(
            std::initializer_list<ID3D11RenderTargetView*> targets,
            std::initializer_list<ID3D11ShaderResourceView*> resources = {},
            std::initializer_list<ID3D11Buffer*> buffers = {},
            ID3D11DepthStencilView * depthStencil = nullptr
        );
        virtual ~RenderPass() {};

        virtual void render() const = 0;
        virtual void update(float deltaTime) = 0;

        const std::vector<ID3D11RenderTargetView*> targets;
        const std::vector<ID3D11ShaderResourceView*> resources;
        const std::vector<ID3D11Buffer*> buffers;
        ID3D11DepthStencilView * depthStencil;

    protected:
        template<typename QueueT>
        void drawInstanced(ID3D11ShaderResourceView * instanceBuffer) const;


    };

    template<typename QueueT>
    void RenderPass::drawInstanced(ID3D11ShaderResourceView * instanceBuffer) const
    {
        Global::context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        Global::context->RSSetState(Global::cStates->CullClockwise());
        Global::context->VSSetShaderResources(10, 1, &instanceBuffer);

        UINT instanceOffset = 0;
        for (auto & pair : RenderQueue::get().getQueue<QueueT>())
        {
            Resources::Models::Files modelId = (Resources::Models::Files)pair.first;
            auto & renderInfos = pair.second;

            HybrisLoader::Model    * model = ModelLoader::get().getModel(modelId);
            HybrisLoader::Mesh     * mesh = &model->getMesh();
            HybrisLoader::Material * material = &model->getMaterial();

            Global::context->VSSetShaderResources(11, 1, mesh->getVertexBuffer());

            ID3D11ShaderResourceView * textures[4] =
            {
                /*Diffuse */ material->getDiffuse(),
                /*Normal  */ material->getNormals(),
                /*Specular*/ material->getSpecular(),
                /*Glow    */ material->getGlow()
            };
            Global::context->PSSetShaderResources(12, 4, textures);

            Global::context->DrawInstanced(
                mesh->getVertexCount(),
                renderInfos.size(),
                0,
                instanceOffset
            );

            instanceOffset += renderInfos.size();
        }

        Global::context->VSSetShaderResources(10, 1, Global::nulls);
        Global::context->VSSetShaderResources(11, 1, Global::nulls);
        Global::context->PSSetShaderResources(12, 4, Global::nulls);

    }
}