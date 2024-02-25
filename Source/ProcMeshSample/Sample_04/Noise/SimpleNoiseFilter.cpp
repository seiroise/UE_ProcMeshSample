// Copyright seiroise.

#include "SimpleNoiseFilter.h"

float USimpleNoiseFilter::Evaluate(const FVector& InPoint) const
{
	float NoiseValue = 0.f;

	float Frequency = m_BaseRoughness;
	float Amplitude = 1.f;

	for(int32 I = 0; I < m_NumLayers; ++I)
	{
		float V = FMath::PerlinNoise3D(InPoint * Frequency + m_NoiseCenter);

		// ノイズの値は[0,1]に収める
		NoiseValue += (V * 0.5f + 0.5f) * Amplitude;

		Frequency *= m_Roughness;
		Amplitude *= m_Persistence;
	}

	// 最小値
	NoiseValue = NoiseValue - m_MinValue;
	
	return NoiseValue * m_Strength;
}
