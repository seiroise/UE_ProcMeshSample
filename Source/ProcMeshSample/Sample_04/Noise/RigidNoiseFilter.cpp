// Copyright seiroise.


#include "RigidNoiseFilter.h"

float URigidNoiseFilter::Evaluate(const FVector& InPoint) const
{
	float NoiseValue = 0.f;
	
	float Frequency = m_BaseRoughness;
	float Amplitude = 1.f;
	float Weight = 1.f;

	for(int32 I = 0; I < m_NumLayers; ++I)
	{
		float V =  FMath::PerlinNoise3D(InPoint * Frequency + m_NoiseCentre);
		// vの絶対値を取って諸々処理
		V = 1. - FMath::Abs(V);
		V *= V;
		V *= Weight;
		Weight = V;
		
		// ノイズの値は[0,1]の範囲に
		NoiseValue += (V + 1) * 0.5f * Amplitude;

		// 各種係数の更新
		Frequency *= m_Roughness;
		Amplitude *= m_Persistence;
	}
	
	// 最小値を適用
	NoiseValue = NoiseValue - m_MinValue;

	// ノイズの強さを反映
	return NoiseValue * m_Strength;
}
