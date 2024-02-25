// Copyright seiroise.

#pragma once

#include "CoreMinimal.h"
#include "PlanetGenerationSettingsDataAsset.h"
#include "ProcMeshSample/Common/MeshDataProxy.h"
#include "UObject/Object.h"

#include "PlanetTerrainFace.generated.h"

class UPlanetGenerationSettingsDataAsset;

/**
 * 頂点データ生成の結果
 */
struct FGenerateVertexDataResult
{
	float m_MinElevation;
	float m_MaxElevation;
};

/**
 * 惑星の地表
 */
UCLASS()
class PROCMESHSAMPLE_API UPlanetTerrainFace : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * 初期化処理
	 */
	void Initialize(int32 InResolution, const FVector& InLocalUp, UPlanetGenerationSettingsDataAsset* InGenSettings);

	/**
	 * 頂点情報の生成
	 * @return 
	 */
	FGenerateVertexDataResult GenerateVertexData();

	/**
	 * 頂点カラーの生成
	 */
	void GenerateVertexColor();
	
	/**
	 * メッシュ情報の構築
	 */
	FMeshDataProxy ConstructMeshData();

	/**
	 * 最小/最大高度を設定
	 */
	void SetMinMaxElevation(float InMinElevation, float InMaxElevation);

private:
	/**
	 * 座標に対するノイズの高さを計算 
	 */
	FPlanetElevationInfo CalculatePlanetElevation(const FVector& InPointOnUnitSphere) const;

	/**
	 * 座標に対応する法線を計算
	 */
	FVector CalculatePlanetNormal(const FVector& InPointOnUnitSphere) const;
	
private:
	
	int32 m_Resolution;
	FVector m_LocalUp;
	FVector m_AxisA;
	FVector m_AxisB;

	float m_MinElevation;
	float m_MaxElevation;

	UPROPERTY()
	TObjectPtr<UPlanetGenerationSettingsDataAsset> m_pGenSettings;

	TArray<FPlanetElevationInfo> m_Elevations;
	TArray<FVector> m_Vertices;
	TArray<FVector> m_Normals;
	TArray<FLinearColor> m_Colors;
};
