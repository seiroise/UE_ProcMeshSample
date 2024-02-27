// Copyright seiroise.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PlanetBiomeSettings.generated.h"

/**
 * 惑星上のバイオームについての設定
 */
UCLASS(EditInlineNew)
class PROCMESHSAMPLE_API UPlanetBiomeSettings : public UObject
{
	GENERATED_BODY()

public:

	/**
	 * 全体の色味
	 */
	UPROPERTY(EditAnywhere)
	FLinearColor m_Tint;
	
	/**
	 * 高さに対応した色情報
	 */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveLinearColor> m_pGradient;

	/**
	 * 色味の割合
	 */
	UPROPERTY(EditAnywhere, meta = (UIMin = 0, UIMax = 1))
	float m_TintPercent = 0.f;

	/**
	 * バイオームの開始緯度。(0が北極、1が南極)
	 */
	UPROPERTY(EditAnywhere, meta = (UIMin = 0, UIMax = 1))
	float m_StartLatitude = 0.f;
};
