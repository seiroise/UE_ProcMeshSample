// Copyright seiroise.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralTerrainChunkManager.generated.h"

class UProceduralTerrainInvokerComponent;
class AProceduralTerrainActor;
/**
 * チャンクの生成管理
 */
UCLASS()
class PROCMESHSAMPLE_API AProceduralTerrainChunkManager : public AActor
{
	GENERATED_BODY()

public:
	AProceduralTerrainChunkManager();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	/**
	 * 基準コンポーネントの登録
	 * @param InInvoker 
	 */
	void RegisterInvoker(UProceduralTerrainInvokerComponent* InInvoker);

	/**
	 * 基準コンポーネントの登録解除
	 * @param InInvoker 
	 */
	void UnregisterInvoker(UProceduralTerrainInvokerComponent* InInvoker);
	
private:
	
	/**
	 * 地形生成用のアクター
	 */
	UPROPERTY(EditAnywhere)
	TObjectPtr<AProceduralTerrainActor> m_pTerrainActor;
	
	/**
	 * チャンク読込の基準となるアクターの配列
	 */
	UPROPERTY()
	TArray<UProceduralTerrainInvokerComponent*> m_InvokerArray;
};
