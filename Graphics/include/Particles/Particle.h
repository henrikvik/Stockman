#pragma once

#include <cstdint>

#include <vector>

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

}