// Copyright seiroise.

#pragma once

#include "CoreMinimal.h"
#include "NoiseFilterBase.h"
#include "RigidNoiseFilter.generated.h"

/**
 * より地形的にそれらしいノイズ生成クラス
 */
UCLASS()
class PROCMESHSAMPLE_API URigidNoiseFilter : public UNoiseFilterBase
{
	GENERATED_BODY()

public:
	virtual float Evaluate(const FVector& InPoint) const override;

public:
	/**
	 * ノイズの強さ
	 */
	UPROPERTY(EditAnywhere)
	float m_Strength = 1.f;

	/**
	 * ノイズのレイヤー数
	 */
	UPROPERTY(EditAnywhere, meta=(UIMin=1, UIMax=20))
	int32 m_NumLayers = 1;

	/**
	 * 基本の粗さ
	 */
	UPROPERTY(EditAnywhere)
	float m_BaseRoughness;

	/**
	 * レイヤー数に応じた粗さの倍率
	 */
	UPROPERTY(EditAnywhere)
	float m_Roughness = 1.f;

	/**
	 * レイヤー数に応じたノイズの強さの倍率
	 */
	UPROPERTY(EditAnywhere)
	float m_Persistence = 0.5f;

	/**
	 * ノイズサンプルの中心座標
	 */
	UPROPERTY(EditAnywhere)
	FVector m_NoiseCentre;

	/**
	 * ノイズの最小値
	 */
	UPROPERTY(EditAnywhere)
	float m_MinValue;
};
