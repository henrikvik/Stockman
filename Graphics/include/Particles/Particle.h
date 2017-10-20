#pragma once

#include <cstdint>

#include <vector>
#include <DirectXMath.h>

namespace Graphics {;

enum class ParticleType : uint32_t {
	Trail = 0,
	Billboard = 1,
	Geometry = 2
};

// Used internally by the format to tween between values
enum class ParticleEase : uint32_t {
	Linear = 0,
	EaseIn = 1,
	EaseOut = 2
};

struct GeometryDefinition {
	uint32_t     m_MaterialIdx;
	float        m_Lifetime;
	float        m_Gravity;
	float        m_NoiseScale;
	float        m_NoiseSpeed;
	ParticleEase m_DeformEasing;
	float        m_DeformStart;
	float        m_DeformEnd;
	float        m_DeformSpeed;
	ParticleEase m_SizeEasing;
	float        m_SizeStart;
	float        m_SizeEnd;
	ParticleEase m_ColorEasing;
	float        m_ColorStart[4];
	float        m_ColorEnd[4];
};

struct ParticleEffectEntry {
	ParticleType m_Type;
	int32_t      m_DefinitionIdx;
	float        m_Start;
	float        m_Time;
	bool         m_Loop;
	float        m_StartPosition[3][2];
	float        m_StartVelocity[3][2];
	ParticleEase m_SpawnEasing;
	float        m_SpawnStart;
	float        m_SpawnEnd;
	float        m_RotLimitMin;
	float        m_RotLimitMax;
	float        m_RotSpeedMin;
	float        m_RotSpeedMax;
};

struct ParticleEffect {
	char     m_Name[16];
	uint32_t m_Count;
	float    m_Time;
	uint32_t m_EntryCount;
	std::vector<ParticleEffectEntry> m_Entries;
};



#define EASE_FUNC(type, name, factor) inline type name(type start, type end, float t) \
{ \
	return Lerp(start, end, factor(t)); \
}

namespace ease {;


inline float Lerp(float start, float end, float t)
{
	return start + t * (end - start);
}

inline DirectX::XMVECTOR Lerp(DirectX::XMVECTOR start, DirectX::XMVECTOR end, float t)
{
	return DirectX::XMVectorLerp(start, end, t);
}

inline float EaseInFactor(float t)
{
	return powf(t, 5);
}

inline float EaseOutFactor(float t)
{
	return 1 - powf(1 - t, 5);
}

EASE_FUNC(float, EaseIn, EaseInFactor)
EASE_FUNC(DirectX::XMVECTOR, EaseIn, EaseInFactor)

EASE_FUNC(float, EaseOut, EaseOutFactor)
EASE_FUNC(DirectX::XMVECTOR, EaseOut, EaseOutFactor)

}

typedef float(*EaseFunc)(float, float, float);
typedef DirectX::XMVECTOR(*EaseFuncV)(DirectX::XMVECTOR, DirectX::XMVECTOR, float);

extern EaseFunc ease_funcs[4];
extern EaseFuncV ease_funcs_xmv[4];

inline EaseFunc GetEaseFunc(ParticleEase ease)
{
	return ease_funcs[(int)ease];
}

inline EaseFuncV GetEaseFuncV(ParticleEase ease)
{
	return ease_funcs_xmv[(int)ease];
}

}