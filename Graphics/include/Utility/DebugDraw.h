#pragma once

#include "../Structs.h"
#include "../Camera.h"
#include "../Particles/ParticleSystem.h"

#include <SimpleMath.h>

namespace Graphics {
namespace Debug {

void Initialize(ID3D11Device *device);
void Destroy();
void Text(DirectX::SimpleMath::Vector3 pos, const char *str);

void ParticleFX(ParticleEffectInstance fx);
void PointLight(Light light);
void Render(Camera *camera);

}
}