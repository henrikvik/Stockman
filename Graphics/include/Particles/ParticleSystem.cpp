#include <tbb\tbb.h>

#include "ParticleSystem.h"

#include <experimental\filesystem>

#include <fstream>
#include <algorithm>
#include <array>

#include <WICTextureLoader.h>
#include <d3dcompiler.h>

#include <Singletons\Profiler.h>
#include "../Utility/DebugDraw.h"
#include "../CommonState.h"
#include <Singletons\DebugWindow.h>
#include "../RenderInfo.h"
#include "../RenderQueue.h"

#ifdef _DEBUG
#define SHADER_COMPILE_FLAGS D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION
#else
#define SHADER_COMPILE_FLAGS D3DCOMPILE_DEBUG
#endif

namespace fs = std::experimental::filesystem;
using namespace DirectX;

std::wstring ConvertToWString(const std::string & s)
{
    const char * cs = s.c_str();
    const size_t wn = std::mbsrtowcs(NULL, &cs, 0, NULL);

    if (wn == size_t(-1))
    {
        return L"";
    }

    std::vector<wchar_t> buf(wn + 1);
    const size_t wn_again = std::mbsrtowcs(buf.data(), &cs, wn + 1, NULL);

    if (wn_again == size_t(-1))
    {
        return L"";
    }

    return std::wstring(buf.data(), wn);
}

class ResourcesShaderInclude : public ID3DInclude
{
public:
    ResourcesShaderInclude(const char* shaderDir) :
        m_ShaderDir(shaderDir)
    {
    }

    HRESULT __stdcall Open(
        D3D_INCLUDE_TYPE IncludeType,
        LPCSTR pFileName,
        LPCVOID pParentData,
        LPCVOID *ppData,
        UINT *pBytes);
    
    HRESULT __stdcall Close(LPCVOID pData);

private:
    std::string m_ShaderDir;
};

HRESULT __stdcall ResourcesShaderInclude::Open(
    D3D_INCLUDE_TYPE IncludeType,
    LPCSTR pFileName,
    LPCVOID pParentData,
    LPCVOID *ppData,
    UINT *pBytes)
{
    std::string finalPath = m_ShaderDir + "\\" + pFileName;

    std::ifstream fileStream(finalPath, std::ios::binary);
    int fileSize = 0;
    fileStream.seekg(0, std::ios::end);
    fileSize = fileStream.tellg();
    fileStream.seekg(0, std::ios::beg);

    if (fileSize)
    {
        char* buf = newd char[fileSize];
        fileStream.read(buf, fileSize);

        *ppData = buf;
        *pBytes = fileSize;
    }
    else
    {
        *ppData = nullptr;
        *pBytes = 0;
    }

    return S_OK;
}

HRESULT __stdcall ResourcesShaderInclude::Close(LPCVOID pData)
{
    char* buf = (char*)pData;
    delete[] buf;
    return S_OK;
}

namespace Graphics {;

GRAPHICS_API ParticleSystem *FXSystem;
bool FXEnabled = true;

ParticleSystem::ParticleSystem(ID3D11Device * device, uint32_t capacity, const char * path)
    : m_Capacity(capacity)
{
    readSphereModel(device);
    readParticleFile(device, path);

    m_GeometryParticles.reserve(capacity);

    DebugWindow::getInstance()->registerCommand("GFX_PARTICLES", [&](std::vector<std::string> &args)->std::string
    {
        FXEnabled = !FXEnabled;

        std::string response = "FXEnabled = ";
        response += FXEnabled ? "true" : "false";

        return response;
    });

}

ParticleSystem::~ParticleSystem()
{
    delete m_SphereVertexBuffer;
    delete m_SphereIndexBuffer;
    delete m_GeometryInstanceBuffer;
    delete m_GeometryVS;

    for (auto &tex : m_Textures) {
        tex->Release();
    }

    for (auto &mat : m_Materials) {
        std::get<0>(mat)->Release();
        std::get<1>(mat)->Release();
    }
}

bool ParticleSystem::processAnchoredEffect(AnchoredParticleEffect *afx, DirectX::SimpleMath::Vector3 pos, float dt)
{
    if (!FXEnabled) return false;
    Debug::ParticleFX({ pos, afx->m_Effect });
    afx->m_Anchor = pos;

    auto fx = &afx->m_Effect;
    for (unsigned int i = 0; i < fx->m_Count; i++) {
        auto &entry = fx->m_Entries[i];

        fx->m_Age += dt;

        if (fx->m_Age < entry.m_Start || fx->m_Age > entry.m_Start + entry.m_Time) {
            if (fx->m_Loop) {
                fx->m_Age = 0.f;
            }
            else {
                continue;
            }
        }

        switch (entry.m_Type) {
            case ParticleType::Geometry: {
                auto def = &m_GeometryDefinitions[entry.m_DefinitionIdx];

                auto factor = (fx->m_Age - entry.m_Start) / entry.m_Time;
                auto ease_spawn = GetEaseFunc(entry.m_SpawnEasing);
                auto spawn = entry.m_Loop ? entry.m_SpawnStart : ease_spawn(entry.m_SpawnStart, entry.m_SpawnEnd, factor);

                for (entry.m_SpawnedParticles += spawn * dt; entry.m_SpawnedParticles >= 1.f; entry.m_SpawnedParticles -= 1.f) {
                    GeometryParticle p = {};
                    p.pos = entry.m_StartPosition.GetPosition();
                    p.velocity = entry.m_StartVelocity.GetVelocity();
                    p.rot = {
                        RandomFloat(entry.m_RotLimitMin, entry.m_RotLimitMax),
                        RandomFloat(entry.m_RotLimitMin, entry.m_RotLimitMax),
                        RandomFloat(entry.m_RotLimitMin, entry.m_RotLimitMax)
                    };
                    p.rotvel = RandomFloat(entry.m_RotSpeedMin, entry.m_RotSpeedMax);
                    p.rotprog = RandomFloat(-180, 180);
                    p.def = def;
                    p.idx = def->m_MaterialIdx;

                    afx->m_Children.push_back(p);
                }
            } break;
            default:
                break;
        }
    }

    return fx->m_Age > fx->m_Time;
}

bool ParticleSystem::processEffect(ParticleEffect * fx, DirectX::SimpleMath::Vector3 pos, float dt)
{
    if (!FXEnabled) return false;
    Debug::ParticleFX({ pos, *fx });

    for (unsigned int i = 0; i < fx->m_Count; i++) {
        auto &entry = fx->m_Entries[i];

        fx->m_Age += dt;

        if (fx->m_Age < entry.m_Start || fx->m_Age > entry.m_Start + entry.m_Time) {
            if (fx->m_Loop) {
                fx->m_Age = 0.f;
            }
            else {
                continue;
            }
        }

        if (fx->m_LightRadius != 0.f) {
            auto col = XMFLOAT4((float*)fx->m_LightColor);

            LightRenderInfo light = {};
            light.position = pos;
            light.range = fx->m_LightRadius;
            light.intensity = col.w;
            light.color.x = col.x;
            light.color.y = col.y;
            light.color.z = col.z;

            QueueRender(light);
        }

        switch (entry.m_Type) {
            case ParticleType::Billboard:
            {
            } break;
            case ParticleType::Geometry: {
                auto def = &m_GeometryDefinitions[entry.m_DefinitionIdx];

                auto factor = (fx->m_Age - entry.m_Start) / entry.m_Time;
                auto ease_spawn = GetEaseFunc(entry.m_SpawnEasing);

                if (entry.m_SpawnStart <= 0.f && entry.m_SpawnEnd <= 0.f) {
                    if (entry.m_SpawnedParticles <= 0.f) {
                        GeometryParticle p = {};
                        p.pos = entry.m_StartPosition.GetPosition() + pos;
                        p.velocity = entry.m_StartVelocity.GetVelocity();
                        p.rot = {
                            RandomFloat(entry.m_RotLimitMin, entry.m_RotLimitMax),
                            RandomFloat(entry.m_RotLimitMin, entry.m_RotLimitMax),
                            RandomFloat(entry.m_RotLimitMin, entry.m_RotLimitMax)
                        };
                        p.rotvel = RandomFloat(entry.m_RotSpeedMin, entry.m_RotSpeedMax);
                        p.rotprog = RandomFloat(-180, 180);
                        p.def = def;
                        p.idx = def->m_MaterialIdx;


                        m_GeometryParticles.push_back(p);

                        entry.m_SpawnedParticles += 1.f;
                    }
                }
                else {
                    auto spawn = entry.m_Loop ? entry.m_SpawnStart : ease_spawn(entry.m_SpawnStart, entry.m_SpawnEnd, factor);

                    for (entry.m_SpawnedParticles += spawn * dt; entry.m_SpawnedParticles >= 1.f; entry.m_SpawnedParticles -= 1.f) {
                        GeometryParticle p = {};
                        p.pos = entry.m_StartPosition.GetPosition() + pos;
                        p.velocity = entry.m_StartVelocity.GetVelocity();
                        p.rot = {
                            RandomFloat(entry.m_RotLimitMin, entry.m_RotLimitMax),
                            RandomFloat(entry.m_RotLimitMin, entry.m_RotLimitMax),
                            RandomFloat(entry.m_RotLimitMin, entry.m_RotLimitMax)
                        };
                        p.rotvel = RandomFloat(entry.m_RotSpeedMin, entry.m_RotSpeedMax);
                        p.rotprog = RandomFloat(-180, 180);
                        p.def = def;
                        p.idx = def->m_MaterialIdx;


                        m_GeometryParticles.push_back(p);
                    }
                }
            } break;
            case ParticleType::Trail: {
            } break;
        }
    }

    return fx->m_Age > fx->m_Time;
}

bool ParticleSystem::processEffect(ParticleEffect * fx, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 velocity, float dt)
{
    if (!FXEnabled) return false;
    Debug::ParticleFX({ pos, *fx });

    for (unsigned int i = 0; i < fx->m_Count; i++) {
        auto &entry = fx->m_Entries[i];

        fx->m_Age += dt;

        if (fx->m_Age < entry.m_Start || fx->m_Age > entry.m_Start + entry.m_Time) {
            if (fx->m_Loop) {
                fx->m_Age = 0.f;
            }
            else {
                continue;
            }
        }

        if (fx->m_LightRadius != 0.f) {
            auto col = XMFLOAT4((float*)fx->m_LightColor);

            LightRenderInfo light = {};
            light.position = pos;
            light.range = fx->m_LightRadius;
            light.intensity = col.w;
            light.color.x = col.x;
            light.color.y = col.y;
            light.color.z = col.z;

            QueueRender(light);
        }

        switch (entry.m_Type) {
        case ParticleType::Billboard:
        {
        } break;
        case ParticleType::Geometry: {
            auto def = &m_GeometryDefinitions[entry.m_DefinitionIdx];

            auto factor = (fx->m_Age - entry.m_Start) / entry.m_Time;
            auto ease_spawn = GetEaseFunc(entry.m_SpawnEasing);
            auto spawn = entry.m_Loop ? entry.m_SpawnStart : ease_spawn(entry.m_SpawnStart, entry.m_SpawnEnd, factor);

            if (entry.m_SpawnStart <= 0.01f && entry.m_SpawnEnd <= 0.01f) {
                if (entry.m_SpawnedParticles <= 0.f) {
                    GeometryParticle p = {};
                    p.pos = entry.m_StartPosition.GetPosition() + pos;
                    p.velocity = velocity + entry.m_StartVelocity.GetVelocity();
                    p.rot = {
                        RandomFloat(entry.m_RotLimitMin, entry.m_RotLimitMax),
                        RandomFloat(entry.m_RotLimitMin, entry.m_RotLimitMax),
                        RandomFloat(entry.m_RotLimitMin, entry.m_RotLimitMax)
                    };
                    p.rotvel = RandomFloat(entry.m_RotSpeedMin, entry.m_RotSpeedMax);
                    p.rotprog = RandomFloat(-180, 180);
                    p.def = def;
                    p.idx = def->m_MaterialIdx;


                    m_GeometryParticles.push_back(p);

                    entry.m_SpawnedParticles += 1.f;
                }
            }
            else {
                for (entry.m_SpawnedParticles += spawn * dt; entry.m_SpawnedParticles >= 1.f; entry.m_SpawnedParticles -= 1.f) {
                    GeometryParticle p = {};
                    p.pos = entry.m_StartPosition.GetPosition() + pos;
                    p.velocity = velocity + entry.m_StartVelocity.GetVelocity();
                    p.rot = {
                        RandomFloat(entry.m_RotLimitMin, entry.m_RotLimitMax),
                        RandomFloat(entry.m_RotLimitMin, entry.m_RotLimitMax),
                        RandomFloat(entry.m_RotLimitMin, entry.m_RotLimitMax)
                    };
                    p.rotvel = RandomFloat(entry.m_RotSpeedMin, entry.m_RotSpeedMax);
                    p.rotprog = RandomFloat(-180, 180);
                    p.def = def;
                    p.idx = def->m_MaterialIdx;


                    m_GeometryParticles.push_back(p);
                }
            }

        } break;
        case ParticleType::Trail: {
        } break;
        }
    }

    return fx->m_Age > fx->m_Time;
}

void ParticleSystem::addEffect(std::string name, XMVECTOR pos)
{
    ParticleEffectInstance effect = {
        pos,
        getEffect(name)
    };
    m_ParticleEffects.push_back(effect);
}

ParticleEffect ParticleSystem::getEffect(std::string name)
{
    // simple linear search
    auto result = std::find_if(m_EffectDefinitions.begin(), m_EffectDefinitions.end(), [name](ParticleEffect &a) {
        return std::strcmp(name.c_str(), a.m_Name) == 0;
    });

    return *result;
}

void ParticleSystem::renderPrePass(ID3D11DeviceContext * cxt, Camera * cam, ID3D11DepthStencilView * dest_dsv)
{
    ID3D11SamplerState *samplers[] = {
        Global::cStates->LinearClamp(),
        Global::cStates->LinearWrap(),
        Global::cStates->PointClamp(),
        Global::cStates->PointWrap()
    };
    cxt->VSSetSamplers(0, 4, samplers);
    cxt->PSSetSamplers(0, 4, samplers);

    cxt->PSSetShaderResources(4, (UINT)m_Textures.size(), m_Textures.data());

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
        auto buf = Global::mainCamera->getBuffer();
        cxt->VSSetConstantBuffers(0, 1, *Global::mainCamera->getBuffer());
        cxt->PSSetConstantBuffers(0, 1, *Global::mainCamera->getBuffer());

        cxt->OMSetDepthStencilState(Global::cStates->DepthDefault(), 0);
        cxt->OMSetRenderTargets(0, nullptr, dest_dsv);
        cxt->RSSetState(Global::cStates->CullNone());

        int offset = 0;
        unsigned int len = 0;
        int current_material = -1;

        for (int i = 0; i < m_FrameGeometryParticles.size(); i++) {
            auto &particle = m_FrameGeometryParticles[i];

            // initialize state during first loop
            if (current_material == -1) {
                current_material = particle.idx;
            }
            // if material changes, flush our built up batching
            else if (current_material != particle.idx) {
                cxt->PSSetShader(std::get<1>(m_Materials[current_material]), nullptr, 0);
                cxt->DrawIndexedInstanced(m_SphereIndices, min(m_Capacity, len), 0, 0, offset);

                current_material = particle.idx;
                offset = i;
                len = 0;
            }

            len++;
        }

        if (len > 0) {
            cxt->PSSetShader(std::get<1>(m_Materials[current_material]), nullptr, 0);
            cxt->DrawIndexedInstanced(m_SphereIndices, min(m_Capacity, len), 0, 0, offset);
        }

        cxt->RSSetState(Global::cStates->CullCounterClockwise());
    }
}

void ParticleSystem::render(
    ID3D11DeviceContext *cxt,
    Camera *cam,
    ID3D11ShaderResourceView *lightIndexList,
    ID3D11ShaderResourceView *lightGrid,
    ID3D11ShaderResourceView *lights,
    ID3D11Buffer *lightBuffer,
    ID3D11ShaderResourceView *shadowMap,
    ID3D11RenderTargetView *dest_rtv, 
    ID3D11DepthStencilView *dest_dsv,
    bool debug)
{
    ID3D11SamplerState *samplers[] = {
        Global::cStates->LinearClamp(),
        Global::cStates->LinearWrap(),
        Global::cStates->PointClamp(),
        Global::cStates->PointWrap()
    };
    cxt->VSSetSamplers(0, 4, samplers);
    cxt->PSSetSamplers(0, 4, samplers);

    ID3D11ShaderResourceView *lightResources[] = {
        lightIndexList,
        lightGrid,
        lights,
        shadowMap
    };
    cxt->PSSetShaderResources(0, 4, lightResources);
    cxt->PSSetShaderResources(4, (UINT)m_Textures.size(), m_Textures.data());
    cxt->PSSetConstantBuffers(0, 1, *Global::mainCamera->getBuffer());

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

        ID3D11Buffer *vertex_buffers[] = {
            *m_SphereVertexBuffer,
            *m_GeometryInstanceBuffer
        };

        cxt->IASetInputLayout(*m_GeometryVS);
        cxt->IASetVertexBuffers(0, 2, vertex_buffers, strides, offsets);
        cxt->IASetIndexBuffer(*m_SphereIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
        cxt->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        cxt->VSSetShader(*m_GeometryVS, nullptr, 0);
        ID3D11Buffer *buffers[] = {
            *cam->getBuffer(),
            lightBuffer
        };
        cxt->VSSetConstantBuffers(0, 1, buffers);
        cxt->PSSetConstantBuffers(0, 2, buffers);

        cxt->OMSetDepthStencilState(Global::cStates->DepthRead(), 0);
        cxt->OMSetRenderTargets(1, &dest_rtv, dest_dsv);
        cxt->RSSetState(Global::cStates->CullNone());

        int offset = 0;
        unsigned int len = 0;
        int current_material = -1;

        for (int i = 0; i < m_FrameGeometryParticles.size(); i++) {
            auto &particle = m_FrameGeometryParticles[i];

            // initialize state during first loop
            if (current_material == -1) {
                current_material = particle.idx;
            }
            // if material changes, flush our built up batching
            else if (current_material != particle.idx) {
                cxt->PSSetShader(std::get<0>(m_Materials[current_material]), nullptr, 0);
                cxt->DrawIndexedInstanced(m_SphereIndices, min(m_Capacity, len), 0, 0, offset);

                current_material = particle.idx;
                offset = i;
                len = 0;
            }

            len++;
        }

        if (len > 0) {
            cxt->PSSetShader(std::get<0>(m_Materials[current_material]), nullptr, 0);
            cxt->DrawIndexedInstanced(m_SphereIndices, min(m_Capacity, len), 0, 0, offset);
        }
        
        cxt->RSSetState(Global::cStates->CullCounterClockwise());
    }
    cxt->PSSetShaderResources(0, 4, Global::nulls);
}

GeometryParticleInstance *ParticleSystem::uploadParticles(std::vector<GeometryParticle> &particles, GeometryParticleInstance *output, GeometryParticleInstance *max)
{
    for (int i = 0; i < particles.size(); i++) {
        if (output >= max) return nullptr;

        auto &particle = particles[i];
        auto def = *particle.def;

        GeometryParticleInstance instance;

        auto factor = particle.age / def.m_Lifetime;


        auto ease_color = GetEaseFuncV(def.m_ColorEasing);
        auto ease_deform = GetEaseFunc(def.m_DeformEasing);
        auto ease_size = GetEaseFunc(def.m_SizeEasing);

        auto scale = ease_size(def.m_SizeStart, def.m_SizeEnd, factor);

        instance.m_Model = XMMatrixRotationAxis(
            XMLoadFloat3(&particle.rot),
            (particle.rotprog + particle.age) *
            particle.rotvel) *
            XMMatrixScaling(scale, scale, scale) *
            XMMatrixTranslationFromVector(XMLoadFloat3(&particle.pos)
        );
        instance.m_Age = factor;

        auto col_start = XMFLOAT4((float*)def.m_ColorStart);
        auto col_end = XMFLOAT4((float*)def.m_ColorEnd);

        instance.m_Color = ease_color(XMLoadFloat4(&col_start), XMLoadFloat4(&col_end), factor);
        instance.m_Deform = ease_deform(def.m_DeformStart, def.m_DeformEnd, factor);
        instance.m_DeformSpeed = def.m_DeformSpeed;
        instance.m_NoiseScale = def.m_NoiseScale;
        instance.m_NoiseSpeed = def.m_NoiseSpeed;
        *output = instance;

        auto ease_light_color = GetEaseFuncV(def.m_LightColorEasing);
        auto ease_light_radius = GetEaseFunc(def.m_LightRadiusEasing);

        auto light_radius = ease_light_radius(def.m_LightRadiusStart, def.m_LightRadiusEnd, factor);
        if (light_radius >=  0.1f) {
            auto light_col_start = XMFLOAT4((float*)def.m_LightColorStart);
            auto light_col_end = XMFLOAT4((float*)def.m_LightColorEnd);

            auto light_color = ease_light_color(XMLoadFloat4(&light_col_start), XMLoadFloat4(&light_col_end), factor);

            LightRenderInfo light;
            light.position = particle.pos;
            light.range = light_radius;
            light.intensity = XMVectorGetW(light_color);
            XMStoreFloat4((XMFLOAT4*)&light.color, light_color);

            QueueRender(light);
        }

        output++;
    }

    return output;
}

void ParticleSystem::updateParticles(XMVECTOR anchor, std::vector<GeometryParticle> &particles, float dt)
{
    auto it = particles.begin();
    while (it != particles.end()) {
        auto def = *it->def;

        it->age += dt;
        it->rotprog += dt;

        if (it->age > def.m_Lifetime) {
            it = particles.erase(it);
            continue;
        }

        XMStoreFloat3(&it->velocity, XMLoadFloat3(&it->velocity) + XMVECTOR { 0, def.m_Gravity, 0 } * dt);
        XMStoreFloat3(&it->pos, XMLoadFloat3(&it->pos) + XMLoadFloat3(&it->velocity) * dt);

        it++;
    }

}

void ParticleSystem::update(ID3D11DeviceContext *cxt, Camera * cam, float dt)
{
    // process all active effects/emitters (spawns particles)
    PROFILE_BEGIN("process managed effects");
    auto it = m_ParticleEffects.begin();
    while (it != m_ParticleEffects.end()) {
        if (processEffect(&it->effect, it->position, dt)) {
            it = m_ParticleEffects.erase(it);
        }
        else {
            it++;
        }
    }
    PROFILE_END();

    // update particles
    PROFILE_BEGIN("update particles");
    updateParticles({}, m_GeometryParticles, dt);
    for (auto fx : m_AnchorParticleEffects) {
        updateParticles(fx->m_Anchor, fx->m_Children, dt);
    }
    PROFILE_END();

    // copy over particles into one big vector for sorting
    PROFILE_BEGIN("copy particle lists")
    m_FrameGeometryParticles = m_GeometryParticles;
    for (auto fx : m_AnchorParticleEffects) {
        m_FrameGeometryParticles.insert(m_FrameGeometryParticles.end(), fx->m_Children.begin(), fx->m_Children.end());
    }
    PROFILE_END();

    // sort by material to improve batching during render
    PROFILE_BEGIN("material sort");
    std::sort(m_FrameGeometryParticles.begin(), m_FrameGeometryParticles.end(), [](GeometryParticle &a, GeometryParticle &b) {
        return a.idx < b.idx;
    });
    PROFILE_END();

    // upload particle instance data to buffer
    PROFILE_BEGIN("upload particles");
    auto base = m_GeometryInstanceBuffer->map(cxt);
    uploadParticles(m_FrameGeometryParticles, base, base + m_Capacity);
    m_GeometryInstanceBuffer->unmap(cxt);
    PROFILE_END();

    m_AnchorParticleEffects.clear();
}

void ParticleSystem::readParticleFile(ID3D11Device *device, const char * path)
{
    auto baseDir = fs::path(path).parent_path().generic_string() + "/";

    // cleanup previous resources, if any
    if (!m_Textures.empty()) {
        for (auto &tex : m_Textures) {
            tex->Release();
        }
        m_Textures.clear();
    }

    if (!m_Materials.empty()) {
        for (auto &mat : m_Materials) {
            std::get<0>(mat)->Release();
            std::get<1>(mat)->Release();
        }
        m_Materials.clear();
    }


    FILE *f = fopen(path, "rb");
    if (!f)
        throw "Can't load particle file";


    // parse header
    const uint8_t FILE_MAGIC[4] = {
        'p', 'a', 'r', 't'
    };
    uint8_t magic[4] = {};
    fread(magic, sizeof(uint8_t), 4, f);

    if (memcmp(magic, FILE_MAGIC, 4) != 0)
        throw "Invalid file magic number";

    uint32_t texture_count = 0;
    fread(&texture_count, sizeof(texture_count), 1, f);

    // load all textures
    for (unsigned int i = 0; i < texture_count; i++) {
        char path[128] = { 0 };
        fread(path, sizeof(char), 128, f);

        ID3D11ShaderResourceView *tex = nullptr;

        std::wstring file = ConvertToWString(baseDir + path);
        auto res = CreateWICTextureFromFile(device, file.c_str(), (ID3D11Resource**)nullptr, &tex);
        if (!SUCCEEDED(res))
            throw "Failed to load texture";

        m_Textures.push_back(tex);
    }

    uint32_t material_count = 0;
    fread(&material_count, sizeof(material_count), 1, f);

    // load all materials
    for (unsigned int i = 0; i < material_count; i++) {
        char path[128] = { 0 };
        fread(path, sizeof(char), 128, f);

        ID3D11PixelShader *ps = nullptr, *psDepth = nullptr;

        ID3DBlob *blob = nullptr, *error = nullptr;
        std::wstring file = ConvertToWString(baseDir + path);

        static ResourcesShaderInclude include("..\\Resources\\Shaders\\");

        auto res = D3DCompileFromFile(
            file.c_str(),
            nullptr,
            &include,
            "PS",
            "ps_5_0",
            SHADER_COMPILE_FLAGS,
            0,
            &blob,
            &error
        );

        if (!SUCCEEDED(res)) {
            OutputDebugStringA((char*)error->GetBufferPointer());

            throw "Failed to compile material shader PS";
        }

        device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &ps);

        res = D3DCompileFromFile(
            file.c_str(),
            nullptr,
            &include,
            "PS_depth",
            "ps_5_0",
            SHADER_COMPILE_FLAGS,
            0,
            &blob,
            &error
        );

        if (!SUCCEEDED(res)) {
            OutputDebugStringA((char*)error->GetBufferPointer());
            throw "Failed to compile material shader PS_depth";
        }

        device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &psDepth);

        m_Materials.push_back(std::make_tuple(ps, psDepth));
    }

    uint32_t geometry_count = 0;
    fread(&geometry_count, sizeof(geometry_count), 1, f);

    // load all geom definitions
    // hopefully works
    m_GeometryDefinitions.clear();
    m_GeometryDefinitions.resize(geometry_count);
    fread(&m_GeometryDefinitions[0], sizeof(GeometryDefinition), geometry_count, f);

    uint32_t fx_count = 0;
    fread(&fx_count, 1, sizeof(fx_count), f);

    // load all particle fx
    for (unsigned int i = 0; i < fx_count; i++) {
        ParticleEffect fx = {};
        fread(fx.m_Name, sizeof(char), 16, f);
        fread(&fx.m_Count, sizeof(fx.m_Count), 1, f);
        fread(&fx.m_Time, sizeof(fx.m_Time), 1, f);
        fread(&fx.m_Loop, sizeof(fx.m_Loop), 1, f);
        fread(&fx.m_LightRadius, sizeof(fx.m_LightRadius), 1, f);
        fread(&fx.m_LightColor, sizeof(fx.m_LightColor), 1, f);

        // default runtime value
        fx.m_Age = 0.f;

        // read all fx entries
        for (unsigned int j = 0; j < fx.m_Count; j++) {

#define LAZY_READ(field) \
    fread(&fx.m_Entries[j].field, sizeof(fx.m_Entries[j].field), 1, f);

            LAZY_READ(m_Type);
            LAZY_READ(m_DefinitionIdx);

            // default runtime value
            fx.m_Entries[j].m_SpawnedParticles = 0.f;

            LAZY_READ(m_Start);
            LAZY_READ(m_Time);
            LAZY_READ(m_Loop);
            LAZY_READ(m_Anchor);
            LAZY_READ(m_StartPosition);
            LAZY_READ(m_StartVelocity);
            LAZY_READ(m_SpawnEasing);
            LAZY_READ(m_SpawnStart);
            LAZY_READ(m_SpawnEnd);
            LAZY_READ(m_RotLimitMin);
            LAZY_READ(m_RotLimitMax);
            LAZY_READ(m_RotSpeedMin);
            LAZY_READ(m_RotSpeedMax);
        }

        m_EffectDefinitions.push_back(fx);
    }
}

void ParticleSystem::readSphereModel(ID3D11Device *device)
{
    std::ifstream meshfile("Resources/Models/sphere.dat", std::ifstream::in | std::ifstream::binary);
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
    m_SphereVertexBuffer = newd Buffer<SphereVertex>(device, BufferUsage::Immutable, BufferBind::VertexBuffer, BufferCpuAccess::None, BufferMisc::None, vertexcount, &vertices[0]);
    m_SphereIndexBuffer = newd Buffer<UINT16>(device, BufferUsage::Immutable, BufferBind::IndexBuffer, BufferCpuAccess::None, BufferMisc::None, indexcount, &indices[0]);

    m_GeometryInstanceBuffer = newd Buffer<GeometryParticleInstance>(device, BufferUsage::Dynamic, BufferBind::VertexBuffer, BufferCpuAccess::Write, BufferMisc::None, m_Capacity);

    m_GeometryVS = newd Shader(device, L"Resources/Shaders/GeometryParticle.hlsl", {
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
