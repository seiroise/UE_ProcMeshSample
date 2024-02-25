// Copyright seiroise.


#include "StepNoiseModifier.h"

#pragma optimize("", off)

float UStepNoiseModifier::ModifyNoise(float InNoiseValue) const
{
	float NoiseValue = InNoiseValue;

	for(int32 I = 0, J = 1; J < m_StepElevation.Num(); I = J++)
	{
		float Min = m_StepElevation[I];
		float Max = m_StepElevation[J];
		
		if(Min <= InNoiseValue && InNoiseValue < Max)
		{
			NoiseValue = Min;
			break;
		}
	}

	return NoiseValue;
}

#pragma optimize("", on)