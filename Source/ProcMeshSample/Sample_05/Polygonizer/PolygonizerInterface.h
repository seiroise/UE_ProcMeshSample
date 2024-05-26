// Copyright seiroise.

#pragma once

#include "CoreMinimal.h"
#include "ProcMeshSample/Common/MeshDataProxy.h"
#include "UObject/Interface.h"
#include "PolygonizerInterface.generated.h"

/**
 * ボリュームデータのポリゴン化を行うための入力データ
 */
struct FPolygonizationInput
{
public:
	/**
	 * スカラー場
	 */
	TArray<float> m_VolumeData;

	/**
	 * スカラー場のグリッド情報
	 */
	FIntVector m_Grid;

	/**
	 * サンプル距離間隔
	 */
	float m_SamplingInterval;
	
	/**
	 * 表面しきい値
	 */
	float m_IsoLevel;
};

/**
 * ポリゴン化の出力データ
 */
struct FPolygonizationOutput
{
public:
	/**
	 * ポリゴン化されたメッシュのデータ
	 */
	FMeshDataProxy m_MeshData;
};

/**
 * ボリュームデータを入力として、三角形分割を行うためのインタフェース
 */
UINTERFACE()
class UPolygonizerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * ボリュームデータを入力として、三角形分割を行うためのインタフェース
 */
class PROCMESHSAMPLE_API IPolygonizerInterface
{
	GENERATED_BODY()

public:
	/**
	 * 入力データの設定と初期化
	 * @param InInput 
	 */
	virtual void Initialize(const FPolygonizationInput& InInput) {}

	/**
	 * ポリゴン化
	 */
	virtual void Polygonize() {}

	/**
	 * 出力データを取得
	 * @param OutOutput 
	 */
	virtual void GetOutput(FPolygonizationOutput& OutOutput) {};
};
