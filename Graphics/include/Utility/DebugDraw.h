#pragma once

#include "../Structs.h"
#include "../Camera.h"
#include "../Particles/ParticleSystem.h"

#include <SimpleMath.h>
#include "../../export.h"

namespace Graphics {
namespace Debug {

void GRAPHICS_API Initialize(ID3D11Device *device);
void GRAPHICS_API Destroy();
void Text(DirectX::SimpleMath::Vector3 pos, const char *str);

void ParticleFX(ParticleEffectInstance fx);
void PointLight(Light light);
void GRAPHICS_API Render(Camera *camera);

}
}