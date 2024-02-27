// Copyright seiroise.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

class UProceduralMeshComponent;

/**
 * メッシュ構築用の一時データ
 */
struct FMeshDataProxy
{

public:
	/**
	 * 三角形の追加
	 * @param InV1 
	 * @param InV2 
	 * @param InV3 
	 */
	void AddTriangle(int32 InV1, int32 InV2, int32 InV3);

	/**
	 * 四辺形の追加。入力頂点は面を正面から見たときに反時計回りになっていること
	 * @param InV1 
	 * @param InV2 
	 * @param InV3 
	 * @param InV4 
	 */
	void AddQuad(int32 InV1, int32 InV2, int32 InV3, int32 InV4);

	/**
	 * データを元にメッシュセクションを作成
	 * @param InMeshComponent 対象のProceduralMeshComponent
	 * @param InMeshSection メッシュセクションの番号
	 * @param InCreateCollision コリジョンを作成するかどうか
	 */
	void CreateMeshSection(UProceduralMeshComponent* InMeshComponent, int32 InMeshSection = 0, bool InCreateCollision = true);
	
public:
	/**
	 * 頂点座標
	 */
	TArray<FVector> m_Vertices;

	/**
	 * 三角形インデックス
	 */
	TArray<int32> m_Triangles;

	/**
	 * 法線
	 */
	bool m_bUseNormals;
	TArray<FVector> m_Normals;

	/**
	 * 頂点カラー
	 */
	bool m_bUseColors;
	TArray<FLinearColor> m_Colors;

	/**
	 * UV
	 */
	bool m_bUseUV1;
	TArray<FVector2D> m_UV1s;
};
