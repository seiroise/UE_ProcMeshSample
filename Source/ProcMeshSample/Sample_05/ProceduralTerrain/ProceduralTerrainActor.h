// Copyright seiroise.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralTerrainActor.generated.h"

class URuntimeTerrainChunkObject;
class UProceduralTerrainEffectorBase;
class UProceduralTerrainProfileDataAsset;
class UNoiseFilterBase;
class UMarchingCubesPolygonizer;

/**
 * 地形データ
 */
struct FTerrainChunkEntry
{
	/**
	 * ボリュームデータ
	 */
	TArray<float> m_VolumeData;
};

/**
 * ボリュームデータによる地形を自動生成するためのアクター
 */
UCLASS()
class PROCMESHSAMPLE_API AProceduralTerrainActor : public AActor
{
	GENERATED_BODY()

public:
	AProceduralTerrainActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	
	/**
	 * 指定のチャンクIDに対応するチャンクを生成する
	 * @param InChunkAddress 
	 */
	void GenerateTerrainChunkAt(const FIntVector& InChunkAddress);

	/**
	 * 指定のアクターの存在するチャンクを生成する
	 * @param InActor 
	 */
	void GenerateTerrainChunkActorLocation(AActor* InActor, int32 InExtent = 1);

	/**
	 * エフェクターの登録
	 * @param InEffector 
	 */
	void RegisterEffector(UProceduralTerrainEffectorBase* InEffector);

	/**
	 * エフェクターの登録解除
	 * @param InEffector 
	 */
	void UnregisterEffector(UProceduralTerrainEffectorBase* InEffector);

	/**
	 * エフェクターの処理
	 * @param InEffector 
	 */
	void ApplyEffector(UProceduralTerrainEffectorBase* InEffector, float InDeltaTime);

private:
	/**
	 * エフェクターの処理
	 * @param InDeltaTime 
	 */
	void ApplyEffectors(float InDeltaTime);
	
	/**
	 * Dirtyなチャンクの更新
	 * @param InDeltaTime 
	 */
	void UpdateDirtyChunks(float InDeltaTime);
	
private:
	/**
	 * 生成基本情報
	 */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UProceduralTerrainProfileDataAsset> m_pProfile;
	
	/**
	 * ポリゴン化用オブジェクト
	 */
	UPROPERTY()
	TObjectPtr<UMarchingCubesPolygonizer> m_pPolygonizer;
	
	/**
	 * 座標に対応するチャンクのマップ
	 */
	UPROPERTY()
	TMap<FIntVector, URuntimeTerrainChunkObject*> m_TerrainChunkMap;

	/**
	 * 適用予定のエフェクター
	 */
	UPROPERTY()
	TArray<UProceduralTerrainEffectorBase*> m_EffectorArray;

	/**
	 * 更新が必要なチャンクの配列
	 */
	UPROPERTY()
	TArray<URuntimeTerrainChunkObject*> m_DirtyChunkArray;
	
#if WITH_EDITOR

	UFUNCTION(CallInEditor, Category="AAA")
	void GenerateTerrainChunkEditor();

#if WITH_EDITORONLY_DATA

	UPROPERTY(EditAnywhere, Category="AAA")
	FIntVector m_ChunkAddressEditorStart;

	UPROPERTY(EditAnywhere, Category="AAA")
	FIntVector m_ChunkAddressEditorEnd;
	
#endif
	
#endif
};
