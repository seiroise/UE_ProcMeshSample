// Copyright seiroise.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlanetGenerationSettingsDataAsset.generated.h"

class UNoiseFilterBase;

/**
 * ノイズレイヤー
 */
USTRUCT()
struct FPlanetNoiseLayer
{
	GENERATED_BODY()

public:

	/**
	 * ノイズが有効か
	 */
	UPROPERTY(EditAnywhere)
	bool m_bIsEnabled = true;

	/**
	 * 一番目のレイヤーをマスクとするか
	 */
	UPROPERTY(EditAnywhere)
	bool m_bUseFirstLayerAsMask;

	/**
	 * ノイズ生成クラス
	 */
	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UNoiseFilterBase> m_pNoiseFilter;
};

/**
 * 惑星の標高情報
 */
struct FPlanetElevationInfo
{
public:
	/**
	 * ノイズ値
	 */
	float m_NoiseValue;
	
	/**
	 * 元々の値
	 */
	float m_Elevation;

	/**
	 * 海面を考慮した値
	 */
	float m_OceanElevation;
};

/**
 * 変更時に呼び出されるデリゲート
 */
DECLARE_MULTICAST_DELEGATE(FOnPlanetGenerationSettingsChangedDelegate)

/**
 * 惑星生成情報設定用のデータアセット
 */
UCLASS()
class PROCMESHSAMPLE_API UPlanetGenerationSettingsDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	/**
	 * 単位球面上のノイズを評価する 
	 */
	float EvaluateNoiseOnUnitSphere(const FVector& InNormal) const;

	/**
	 * 惑星表面の標高を求める
	 * @param InNormal 
	 * @return 
	 */
	FPlanetElevationInfo CalculatePlanetElevation(const FVector& InNormal) const;

	/**
	 * ノイズレイヤーの配列を返す
	 */
	TArray<FPlanetNoiseLayer>& GetNoiseLayerArray() { return m_NoiseLayerArray; }
	
private:
	
	/**
	 * 惑星の半径
	 */
	UPROPERTY(EditAnywhere, meta=(UIMin = 100))
	float m_PlanetRadius = 500.f;
	
	/**
	 * 各種ノイズ設定
	 */
	UPROPERTY(EditAnywhere)
	TArray<FPlanetNoiseLayer> m_NoiseLayerArray;

#if WITH_EDITOR

public:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	FOnPlanetGenerationSettingsChangedDelegate OnChanged;
	
private:
#endif
};
