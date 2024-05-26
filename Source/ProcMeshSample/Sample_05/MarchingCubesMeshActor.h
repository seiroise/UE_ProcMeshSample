// Copyright seiroise.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProcMeshSample/Common/IntRange3D.h"
#include "ProcMeshSample/Common/MeshDataProxy.h"
#include "MarchingCubesMeshActor.generated.h"

struct FMeshDataProxy;
/**
 * MarchingCubes法によるメッシュの生成
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROCMESHSAMPLE_API AMarchingCubesMeshActor : public AActor
{
	GENERATED_BODY()

public:
	AMarchingCubesMeshActor();

protected:
	virtual void BeginPlay() override;

private:
	/**
	 * グリッドの初期化
	 */
	void InitializeGrid();
	
	/**
	 * MarchingCubesでメッシュを生成する
	 */
	void GenerateMarchingCubesMesh();

	/**
	 * 対応する格子の処理
	 * @param InPos
	 */
	void MarchCube(float InIsoLevel, const FIntVector& InPos);

	/**
	 * 頂点の補間
	 * @param InIsoLevel 
	 * @param InP1 
	 * @param InP2 
	 * @param InV1 
	 * @param InV2 
	 * @return 
	 */
	FVector InterpVertex(float InIsoLevel, const FVector& InP1, const FVector& InP2, float InV1, float InV2);
	
private:
	/**
	 * メッシュコンポーネント
	 */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProceduralMeshComponent> m_pProcMeshComponent;
	
	/**
	 * １つのセルの大きさ
	 */
	UPROPERTY(EditAnywhere)
	float m_CellSize = 100.f;

	/**
	 * グリッド数
	 */
	UPROPERTY(EditAnywhere)
	int32 m_GridCount = 4;

	/**
	 * 等値面しきい値
	 */
	UPROPERTY(EditAnywhere)
	float m_IsoLevel = 0.5f;
	
	/**
	 * 密度入力データ
	 */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UDataTable> m_pDensityInput;

	/**
	 * グリッド範囲
	 */
	FIntRange3D m_GridRange;
	
	/**
	 * グリッドを構成する頂点情報
	 */
	TArray<float> m_GridArray;

	/**
	 * メッシュデータ
	 */
	FMeshDataProxy m_MeshData;

#if WITH_EDITOR

	UFUNCTION(CallInEditor)
	void GenerateMarchingCubesMeshEditor();
#endif
};
