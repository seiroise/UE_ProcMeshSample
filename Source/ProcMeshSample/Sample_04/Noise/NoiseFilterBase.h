// Copyright seiroise.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "NoiseFilterBase.generated.h"

/**
 * ノイズ生成用の基底クラス
 */
UCLASS(EditInlineNew)
class PROCMESHSAMPLE_API UNoiseFilterBase : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * 座標に対応するノイズを生成する
	 * @param InPoint 
	 * @return 
	 */
	virtual float Evaluate(const FVector& InPoint) const { return 0.f; }
};
