// Copyright seiroise.

#pragma once

#include "CoreMinimal.h"
#include "NoiseModifierBase.h"

#include "StepNoiseModifier.generated.h"

/**
 * 一定高度毎にステップを作成するModifier
 */
UCLASS()
class PROCMESHSAMPLE_API UStepNoiseModifier : public UNoiseModifierBase
{
	GENERATED_BODY()

public:
	virtual float ModifyNoise(float InNoiseValue) const override;

private:

	UPROPERTY(EditAnywhere)
	TArray<float> m_StepElevation;
};
