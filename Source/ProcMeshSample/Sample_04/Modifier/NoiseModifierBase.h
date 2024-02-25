// Copyright seiroise.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "NoiseModifierBase.generated.h"

struct FPlanetElevationInfo;

/**
 * 高さの出力を変更するための基底クラス
 */
UCLASS(EditInlineNew)
class PROCMESHSAMPLE_API UNoiseModifierBase : public UObject
{
	GENERATED_BODY()

public:
	virtual float ModifyNoise(float InNoiseValue) const { return InNoiseValue; }
};
