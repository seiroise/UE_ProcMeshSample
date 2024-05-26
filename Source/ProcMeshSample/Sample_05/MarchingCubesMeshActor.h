// Copyright seiroise.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProcMeshSample/Common/IntRange3D.h"
#include "ProcMeshSample/Common/MeshDataProxy.h"
#include "MarchingCubesMeshActor.generated.h"

class UMarchingCubesPolygonizer;
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
	 * MarchingCubesでメッシュを生成する
	 */
	void GenerateMarchingCubesMesh();

	/**
	 * 球体状のボリュームデータを生成する
	 * @param InSize 
	 * @param OutVolumeData 
	 */
	void GenerateSphereVolumeData(int32 InSize, TArray<float>& OutVolumeData);
	
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
	 * ポリゴン化処理オブジェクト
	 */
	UPROPERTY()
	TObjectPtr<UMarchingCubesPolygonizer> m_pPolygonizer;

#if WITH_EDITOR

	UFUNCTION(CallInEditor)
	void GenerateMarchingCubesMeshEditor();
#endif
};
