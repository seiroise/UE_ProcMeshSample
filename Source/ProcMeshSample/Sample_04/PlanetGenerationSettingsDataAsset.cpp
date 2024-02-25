// Copyright seiroise.


#include "PlanetGenerationSettingsDataAsset.h"

#include "Modifier/NoiseModifierBase.h"
#include "Noise/NoiseFilterBase.h"

float UPlanetGenerationSettingsDataAsset::EvaluateNoiseOnUnitSphere(const FVector& InNormal) const
{
	if(m_NoiseLayerArray.IsEmpty())
	{
		return 0.f;
	}

	float NoiseValue = 0.f;
	
	// 最初のレイヤーの値をキープ
	float FirstLayerValue = 0.f;
	if(m_NoiseLayerArray.Num() > 0)
	{
		const FPlanetNoiseLayer& NoiseLayer = m_NoiseLayerArray[0];
		if(IsValid(NoiseLayer.m_pNoiseFilter))
		{
			FirstLayerValue = NoiseLayer.m_pNoiseFilter->Evaluate(InNormal);	
		}
		if(NoiseLayer.m_bIsEnabled)
		{
			NoiseValue = FirstLayerValue;
		}
	}
	
	for(int32 I = 1; I < m_NoiseLayerArray.Num(); ++I)
	{
		const FPlanetNoiseLayer& NoiseLayer = m_NoiseLayerArray[I];
		
		// 有効になっているノイズ設定のみ反映
		if(!NoiseLayer.m_bIsEnabled)
		{
			continue;
		}
		
		if(IsValid(NoiseLayer.m_pNoiseFilter))
		{
			float Mask = NoiseLayer.m_bUseFirstLayerAsMask  ? FirstLayerValue : 1.f;
			NoiseValue += NoiseLayer.m_pNoiseFilter->Evaluate(InNormal) * Mask;	
		}
	}

	return NoiseValue;
}

FPlanetElevationInfo UPlanetGenerationSettingsDataAsset::CalculatePlanetElevation(const FVector& InNormal) const
{
	FPlanetElevationInfo ElevationInfo;
	float NoiseValue = EvaluateNoiseOnUnitSphere(InNormal);

	// ノイズ編集
	for(const FPlanetNoiseModifier& Modifier : m_NoiseModifierArray)
	{
		if(Modifier.m_bIsEnabled && IsValid(Modifier.m_pNoiseModifier))
		{
			NoiseValue = Modifier.m_pNoiseModifier->ModifyNoise(NoiseValue);
		}
	}
	
	float ClampedNoiseValue = FMath::Max(NoiseValue, 0.f);
	
	// ノイズ情報
	ElevationInfo.m_NoiseValue = NoiseValue;
	// 標高
	ElevationInfo.m_Elevation = (NoiseValue + 1) * m_PlanetRadius;
	// 海面からの標高
	ElevationInfo.m_OceanElevation = (ClampedNoiseValue + 1) * m_PlanetRadius;
	return ElevationInfo;
}

#if WITH_EDITOR

void UPlanetGenerationSettingsDataAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	OnChanged.Broadcast();
}

#endif
