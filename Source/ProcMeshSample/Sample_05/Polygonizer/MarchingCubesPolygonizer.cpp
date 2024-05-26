// Copyright seiroise.


#include "MarchingCubesPolygonizer.h"

#include "ProcMeshSample/Sample_05/MarchingCubesConsts.h"

void UMarchingCubesPolygonizer::Initialize(const FPolygonizationInput& InInput)
{
	// 格子の初期化
	m_IsoLevel = InInput.m_IsoLevel;
	m_GridRange = FIntRange3D{InInput.m_Grid};
	m_SamplingInterval = InInput.m_SamplingInterval;
	m_VolumeData = InInput.m_VolumeData;
}

void UMarchingCubesPolygonizer::Polygonize()
{
	// メッシュデータ初期化
	m_MeshData.Reset();
	m_MeshData.m_bUseNormals = true;

	// 1段階縮小した範囲をマーチする
	FIntRange3D MarchRange = m_GridRange.ExpandMax(-1);
	for (const FIntVector& Pos : MarchRange)
	{
		MarchCube(m_IsoLevel, Pos);
	}
}

void UMarchingCubesPolygonizer::GetOutput(FPolygonizationOutput& OutOutput)
{
	// メッシュデータをそのまま割当
	OutOutput.m_MeshData = m_MeshData;
}

void UMarchingCubesPolygonizer::MarchCube(float InIsoLevel, const FIntVector& InPos)
{
	// 立方体の対応するキューブテーブル番号を求める
	int32 CubeIndex = 0;
	for (int32 I = 0; I < 8; ++I)
	{
		if (m_VolumeData[m_GridRange.PosToIndex(InPos + MarchingCubesConsts::CubeVertices[I])] < InIsoLevel)
		{
			CubeIndex |= (1 << I);
		}
	}

	// 格子が完全に内部または外部にある場合はスルー
	int32 EdgeMask = MarchingCubesConsts::EdgeTable[CubeIndex];
	if (EdgeMask == 0)
	{
		return;
	}

	// 辺に対応する座標を求める
	static TArray<FVector> EdgeVertArray;
	EdgeVertArray.Init(FVector::ZeroVector, 12);
	// 12個の辺それぞれのマスクを調べる
	for (int32 I = 0; I < 12; ++I)
	{
		// 交差座標の発生する辺かどうか
		if (EdgeMask & (1 << I))
		{
			// 対応する格子の座標番号を求める
			FIntVector PosIndex1 = InPos + MarchingCubesConsts::CubeVertices[MarchingCubesConsts::EdgeComb[I][0]];
			FIntVector PosIndex2 = InPos + MarchingCubesConsts::CubeVertices[MarchingCubesConsts::EdgeComb[I][1]];

			// 格子点の座標
			FVector P1 = FVector(PosIndex1) * m_SamplingInterval;
			FVector P2 = FVector(PosIndex2) * m_SamplingInterval;

			// 格子点の値
			float V1 = m_VolumeData[m_GridRange.PosToIndex(PosIndex1)];
			float V2 = m_VolumeData[m_GridRange.PosToIndex(PosIndex2)];

			// 補間した座標を求める
			EdgeVertArray[I] = InterpVertex(InIsoLevel, P1, P2, V1, V2);
		}
	}

	// TriTableを元に三角形の生成
	for (int32 I = 0; MarchingCubesConsts::TriTable[CubeIndex][I] != -1; I += 3)
	{
		// 座標
		int32 Idx1 = m_MeshData.m_Vertices.Emplace(EdgeVertArray[MarchingCubesConsts::TriTable[CubeIndex][I]]);
		int32 Idx2 = m_MeshData.m_Vertices.Emplace(EdgeVertArray[MarchingCubesConsts::TriTable[CubeIndex][I + 1]]);
		int32 Idx3 = m_MeshData.m_Vertices.Emplace(EdgeVertArray[MarchingCubesConsts::TriTable[CubeIndex][I + 2]]);
		// 法線
		FVector Normal = FVector::CrossProduct(
			m_MeshData.m_Vertices[Idx3] - m_MeshData.m_Vertices[Idx2],
			m_MeshData.m_Vertices[Idx2] - m_MeshData.m_Vertices[Idx1]
		).GetSafeNormal();
		m_MeshData.m_Normals.Emplace(Normal);
		m_MeshData.m_Normals.Emplace(Normal);
		m_MeshData.m_Normals.Emplace(Normal);
		// インデックス
		m_MeshData.AddTriangle(Idx1, Idx2, Idx3);
	}
}

FVector UMarchingCubesPolygonizer::InterpVertex(float InIsoLevel, const FVector& InP1, const FVector& InP2, float InV1,
                                                float InV2)
{
	float T = (InIsoLevel - InV1) / (InV2 - InV1);
	return InP1 + (InP2 - InP1) * T;
}
