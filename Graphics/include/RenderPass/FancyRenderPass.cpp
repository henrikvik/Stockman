#include "FancyRenderPass.h"
#include <WICTextureLoader.h>

namespace Graphics {

FancyRenderPass::FancyRenderPass(
        ID3D11RenderTargetView *output,
        ID3D11ShaderResourceView *lightGrid,
        ID3D11ShaderResourceView *lightIndexList,
        ID3D11ShaderResourceView *lights,
        ID3D11ShaderResourceView *shadowMap,
        ID3D11Buffer *lightBuffer,
        ID3D11DepthStencilView *depthStencil)
    : RenderPass({}, {}, {}, depthStencil),
      m_Output(output),
      m_LightGrid(lightGrid),
      m_LightIndexList(lightIndexList),
      m_Lights(lights),
      m_LightBuffer(lightBuffer),
      m_ShadowMap(shadowMap)
{
    std::vector<DirectX::SpriteFont::Glyph> glyphs;
    FILE *f = fopen("../Resources/Particles/SPLASH.fnt", "r");
    if (f) {
        DirectX::SpriteFont::Glyph glyph = {};

        int x, y, w, h;
        int xadv, xoff, yoff;
        int status = 0;
        printf("%d\n", status);
        while (status = fscanf_s(f, "char id=%d x=%d y=%d width=%d height=%d xoffset=%d yoffset=%d xadvance=%d page=0 chnl=15\n", &glyph.Character, &x, &y, &w, &h, &xoff, &yoff, &xadv), status != 0 && status != EOF) {
            glyph.XOffset = xoff;
            glyph.YOffset = yoff;
            glyph.XAdvance = 0;// xadv;
            glyph.Subrect.left = x;
            glyph.Subrect.right = x + w;
            glyph.Subrect.top = y;
            glyph.Subrect.bottom = y + h;

            glyphs.push_back(glyph);
        }
    }

    ThrowIfFailed(DirectX::CreateWICTextureFromFile(Global::device, L"../Resources/Particles/SPLASH.png", nullptr, &m_FontTexture));
    m_Font = newd DirectX::SpriteFont(m_FontTexture, glyphs.data(), glyphs.size(), 0.f);
}

FancyRenderPass::~FancyRenderPass()
{
    RenderPass::~RenderPass();
    delete m_Font;
}

static DirectX::SimpleMath::Vector4 TransformScreen(DirectX::SimpleMath::Matrix vp, DirectX::SimpleMath::Vector3 position, bool *clip)
{
    using namespace DirectX::SimpleMath;

    auto vec = Vector4(position);
    vec.w = 1;
    auto pos = Vector4::Transform(vec, vp);

    // varför?
    if (pos.w == 0.f) pos.w = 1.f;

    pos.z = (pos.z + pos.w) * 0.5f;
    pos /= pos.w;
    pos = (pos * 0.5f) + Vector4(0.5f);
    if (clip && pos.z > 1.f) *clip = true;
    pos.y = 1 - pos.y;
    pos.x *= WIN_WIDTH;
    pos.y *= WIN_HEIGHT;
    return pos;
}

void FancyRenderPass::render() const
{
    using namespace DirectX::SimpleMath;

    auto view = Global::mainCamera->getView();
    auto proj = Global::mainCamera->getProj();

    Global::context->OMSetRenderTargets(1, &m_Output, nullptr);

    Global::batch->Begin(
        DirectX::SpriteSortMode::SpriteSortMode_Deferred,
        Global::cStates->NonPremultiplied(),
        nullptr,
        Global::cStates->DepthRead(),
        Global::cStates->CullNone(),
        nullptr
    );

    for (auto &info : RenderQueue::get().getQueue<FancyRenderInfo>()) {

        auto origin = (DirectX::XMVECTOR)m_Font->MeasureString(info.text.c_str());
        origin = Vector2(origin) * Vector2{ 0.5f, 0.5f };

        bool clip = false;
        auto pos = TransformScreen(view * proj, info.position, &clip);// Vector3::Transform(info.position, proj * view);
                                                                        //auto pos = Vector3::Transform(info.position, view * proj);
        if (clip) continue;

        pos.x = roundf(pos.x);
        pos.y = roundf(pos.y);

        auto maxdist = 100.f;
        auto dist = (info.position - Global::mainCamera->getPos()).Length();
        auto factor = dist / maxdist;
        if (factor > 1.f) factor = 1.f;

        info.scale = (1 - factor) * info.scale;
        info.color.w = 1 - factor;

        m_Font->DrawString(
            Global::batch,
            info.text.c_str(),
            (DirectX::XMVECTOR) pos,
            (DirectX::FXMVECTOR)info.color,
            0.f,
            origin,
            DirectX::XMVECTOR { info.scale, info.scale },
            DirectX::SpriteEffects_None,
            1.f
        );
    }

    Global::batch->End();

    Global::context->OMSetRenderTargets(0, nullptr, nullptr);
}

void FancyRenderPass::update(float deltaTime)
{
}

}
