// Copyright seiroise.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralTerrainEffectorBase.h"
#include "TerrainEffector_Sphere.generated.h"

/**
 * 球状のTerrainEffector
 */
UCLASS()
class PROCMESHSAMPLE_API UTerrainEffector_Sphere : public UProceduralTerrainEffectorBase
{
	GENERATED_BODY()

public:
	virtual FBox GetEffectRange() const override;
	virtual void Apply(URuntimeTerrainChunkObject* InChunk, float InDeltaTime) override;

public:
	/**
	 * 中心座標
	 */
	UPROPERTY(EditAnywhere)
	FVector m_Location;

	/**
	 * 半径
	 */
	UPROPERTY(EditAnywhere)
	float m_Radius;

	/**
	 * 効果スケール
	 */
	UPROPERTY(EditAnywhere)
	float m_EffectStrength;
};
