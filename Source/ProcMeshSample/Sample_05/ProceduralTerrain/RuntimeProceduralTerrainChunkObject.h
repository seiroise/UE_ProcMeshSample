// Copyright seiroise.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RuntimeProceduralTerrainChunkObject.generated.h"

class IPolygonizerInterface;
class UProceduralTerrainProfileDataAsset;

/**
 * 実行時に生成されたチャンクを管理するためのオブジェクト
 */
UCLASS()
class URuntimeTerrainChunkObject : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * 番号に対応するグリッド交点の座標を取得する
	 * @param InPosIndex
	 * @return 
	 */
	FVector GetGridLocationAt(const FIntVector& InPosIndex) const;

	/**
	 * 番号に対応するグリッドの値を取得する
	 * @param InIndex 
	 * @return 
	 */
	float GetGridValueAt(int32 InIndex) const;

	/**
	 * 番号に対応するグリッドの値を設定する
	 * @param InIndex 
	 * @param InNewValue 
	 */
	void SetGridValueAt(int32 InIndex, float InNewValue);

	/**
	 * ボリュームデータ更新時の処理
	 */
	void OnVolumeDataUpdated(IPolygonizerInterface* InPolygonizer);
	
public:
	/**
	 * 生成基本情報
	 */
	UPROPERTY()
	TObjectPtr<UProceduralTerrainProfileDataAsset> m_pProfile;
	
	/**
	 * チャンクの原点
	 */
	FVector m_Origin;
	
	/**
	 * 更新フラグ
	 */
	bool m_bIsDirty;
	
	/**
	 * ボリュームデータ
	 */
	TArray<float> m_VolumeData;

	/**
	 * チャンクに対応しているアクター
	 */
	UPROPERTY()
	TObjectPtr<AActor> m_pActor;
};