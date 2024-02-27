// Copyright seiroise.


#include "PlanetGenerationSettingsDataAsset.h"

#include "Biome/PlanetBiomeSettings.h"
#include "Curves/CurveLinearColor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Modifier/NoiseModifierBase.h"
#include "Noise/NoiseFilterBase.h"

#pragma optimize("", off)

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

float UPlanetGenerationSettingsDataAsset::CalculateBiomeIndex(const FVector& InNormal) const
{
	// バイオームが設定されていない場合はデフォルト値
	if(m_BiomeArray.IsEmpty())
	{
		return 0.f;
	}

	FLinearColor Color = FLinearColor::Black;
	
	// 緯度(Zについて[1,-1]を[0,1]になるようにマッピング)
	float Latitude = 1.f - (InNormal.Z * 0.5f + 0.5f);

	float BiomeWeight = 0.f;

	for(int32 I = 0; I <  m_BiomeArray.Num(); ++I)
	{
		const FPlanetBiomeEntry& BiomeEntry = m_BiomeArray[I];
		if(!IsValid(BiomeEntry.m_pBiomeSettings))
		{
			continue;
		}

		float BiomeLatitude = Latitude - BiomeEntry.m_pBiomeSettings->m_StartLatitude;
		// Weight = 1でBiomeの中心 
		float Weight = FMath::Clamp(UKismetMathLibrary::NormalizeToRange(BiomeLatitude, -m_BiomeBlendRange, m_BiomeBlendRange), 0.f, 1.f);
		// Weight = 1.f - FMath::Abs(Weight * 2.f - 1.f);
		
		// Weight = 0の場合はこの前のBiomeなので、それのWeightを残す。
		// Weight = 1の場合はこの後のBiomeになるのでそれまでのBiomeは0になる。
		// Weight = 0.5の場合はこれまでのBiomeのWeightを半分にする。
		BiomeWeight *= (1.f - Weight);
		BiomeWeight += I * Weight;
	}
	
	return BiomeWeight;
}

float UPlanetGenerationSettingsDataAsset::CalculateBiomeTextureCoordinate(const FVector& InNormal) const
{
	float BiomeWeight = CalculateBiomeIndex(InNormal);

	return (BiomeWeight + 0.5f) / (m_BiomeArray.Num() - 1);
}

FLinearColor UPlanetGenerationSettingsDataAsset::CalculatePlanetColor(const FVector& InNormal, float InElevationRatio) const
{
	float BiomeWeight = CalculateBiomeIndex(InNormal);
	int32 Index_BiomeA = FMath::FloorToInt(BiomeWeight);
	float Alpha_BiomeA = BiomeWeight - Index_BiomeA;
	int32 Index_BiomeB = FMath::Min(Index_BiomeA + 1, m_BiomeArray.Num() - 1);
	
	const UPlanetBiomeSettings* BiomeA = m_BiomeArray[Index_BiomeA].m_pBiomeSettings;
	const UPlanetBiomeSettings* BiomeB = m_BiomeArray[Index_BiomeB].m_pBiomeSettings;
	
	FLinearColor Color_BiomeA = FLinearColor::White;
	if(IsValid(BiomeA))
	{
		Color_BiomeA = BiomeA->m_Tint;
		if(IsValid(BiomeA->m_pGradient))
		{
			Color_BiomeA *= BiomeA->m_pGradient->GetLinearColorValue(InElevationRatio);	
		}
	}
	FLinearColor Color_BiomeB = FLinearColor::White;
	if(IsValid(BiomeB))
	{
		Color_BiomeB = BiomeB->m_Tint;
		if(IsValid(BiomeB->m_pGradient))
		{
			Color_BiomeB *= BiomeB->m_pGradient->GetLinearColorValue(InElevationRatio);	
		}
	}

	// ブレンドした結果を返す
	return FMath::Lerp(Color_BiomeA, Color_BiomeB, Alpha_BiomeA);
}

#if WITH_EDITOR

void UPlanetGenerationSettingsDataAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	OnChanged.Broadcast();
}

#endif

#pragma optimize("", on)