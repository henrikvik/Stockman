#pragma once

#include "../Structs.h"
#include "../Camera.h"
#include "../Particles/ParticleSystem.h"

#include "../DLL.h"

#include <SimpleMath.h>
#include "../../export.h"

namespace Graphics {
namespace Debug {

void GRAPHICS_API Initialize(ID3D11Device *device);
void GRAPHICS_API Destroy();
void GRAPHICS_API Text(DirectX::SimpleMath::Vector3 pos, ImColor color, std::string text);

void ParticleFX(ParticleEffectInstance fx);
void PointLight(Light light);
void GRAPHICS_API Render(Camera *camera);

}
}