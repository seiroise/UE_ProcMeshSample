// Copyright seiroise.

#pragma once

#include "CoreMinimal.h"
#include "PolygonizerInterface.h"
#include "ProcMeshSample/Common/IntRange3D.h"
#include "UObject/Object.h"
#include "MarchingCubesPolygonizer.generated.h"

/**
 * MarchingCubesによるボリュームデータのポリゴン化を行うためのクラス
 */
UCLASS()
class PROCMESHSAMPLE_API UMarchingCubesPolygonizer
	: public UObject
	, public IPolygonizerInterface
{
	GENERATED_BODY()

public:
	/** Begin IPolygonizerInterface Interface */
	virtual void Initialize(const FPolygonizationInput& InInput) override;
	virtual void Polygonize() override;
	virtual void GetOutput(FPolygonizationOutput& OutOutput) override;
	/** End IPolygonizerInterface Interface */

private:

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
	 * 表面しきい値
	 */
	float m_IsoLevel;
	
	/**
	 * グリッド範囲
	 */
	FIntRange3D m_GridRange;

	/**
	 * メッシュデータ
	 */
	FMeshDataProxy m_MeshData;

	/**
	 * サンプル間隔
	 */
	float m_SamplingInterval;

	/**
	 * ボリュームデータ
	 */
	TArray<float> m_VolumeData;
};
