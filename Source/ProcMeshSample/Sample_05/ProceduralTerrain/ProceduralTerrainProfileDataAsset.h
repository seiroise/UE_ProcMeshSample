// Copyright seiroise.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ProceduralTerrainProfileDataAsset.generated.h"

class UNoiseFilterBase;

/**
 * Procedural Terrainの生成基本情報
 */
UCLASS()
class PROCMESHSAMPLE_API UProceduralTerrainProfileDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	/**
	 * 座標に対応するチャンクアドレスを取得(座標はFloorされた値を使用)
	 * @param InLocation 
	 * @return 
	 */
	FIntVector GetChunkAddressFromLocationFloor(const FVector& InLocation) const;

	/**
	 * 座標に対するチャンクアドレスを取得(座標はRoundされた値を使用)
	 * @param InLocation 
	 * @return 
	 */
	FIntVector GetChunkAddressFromLocationRound(const FVector& InLocation) const;

public:
	/**
	 * 生成シード値
	 */
	UPROPERTY(EditAnywhere)
	int32 m_Seed = 0;

	/**
	 * チャンクの大きさ
	 */
	UPROPERTY(EditAnywhere)
	int32 m_ChunkSize = 16;

	/**
	 * 表面しきい値
	 */
	UPROPERTY(EditAnywhere)
	float m_IsoLevel = 0.5f;

	/**
	 * １つのセルの大きさ
	 */
	UPROPERTY(EditAnywhere)
	float m_CellSize = 100.f;

	/**
	 * ノイズサンプリング用の座標のスケール
	 */
	UPROPERTY(EditAnywhere)
	float m_NoiseSamplingScale = 1.f;

	/**
	 * ノイズフィルター
	 */
	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UNoiseFilterBase> m_pNoiseFilter;
};
