// Copyright seiroise.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ProceduralTerrainEffectorBase.generated.h"

class URuntimeTerrainChunkObject;
/**
 * 地形に影響を与えるためのクラスの基底
 */
UCLASS()
class PROCMESHSAMPLE_API UProceduralTerrainEffectorBase : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * 影響範囲を取得
	 * @return 
	 */
	virtual FBox GetEffectRange() const;

	/**
	 * 影響を適用
	 * @param InChunk 
	 */
	virtual void Apply(URuntimeTerrainChunkObject* InChunk ,float InDeltaTime);
};
