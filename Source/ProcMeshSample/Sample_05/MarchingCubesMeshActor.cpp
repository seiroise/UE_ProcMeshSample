// Copyright seiroise.


#include "MarchingCubesMeshActor.h"

#include "MarchingCubesConsts.h"
#include "ProceduralMeshComponent.h"
#include "ProcMeshSample/Common/IntRange3D.h"

AMarchingCubesMeshActor::AMarchingCubesMeshActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	SetRootComponent(RootComponent);

	m_pProcMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Mesh"));
	m_pProcMeshComponent->SetupAttachment(GetRootComponent());
}

void AMarchingCubesMeshActor::BeginPlay()
{
	Super::BeginPlay();
}

void AMarchingCubesMeshActor::InitializeGrid()
{
	m_GridRange = {m_GridCount};
	m_GridArray.Init(0, m_GridRange.GetPosNum());

	for (const FIntVector& Pos : m_GridRange)
	{
		// 範囲の中心の球体として、そこからの距離で初期化
		int32 Index = m_GridRange.PosToIndex(Pos);
		float Distance = (FVector(Pos) - m_GridRange.GetCenterFloat() / m_GridRange.GetSizeFloat()).Length();
		m_GridArray[Index] = Distance;
	}
}

void AMarchingCubesMeshActor::GenerateMarchingCubesMesh()
{
	// メッシュデータ初期化
	m_MeshData.m_Vertices.Reset();
	m_MeshData.m_Triangles.Reset();
	m_MeshData.m_bUseColors = false;
	m_MeshData.m_bUseNormals = false;
	m_MeshData.m_bUseUV1 = false;
	
	// 1段階縮小した範囲をマーチする
	FIntRange3D MarchRange = m_GridRange.ExpandMax(-1);
	for (const FIntVector& Pos : MarchRange)
	{
		MarchCube(m_IsoLevel, Pos);
	}

	// メッシュを生成
	m_MeshData.CreateMeshSection(m_pProcMeshComponent, 0, false);
}

void AMarchingCubesMeshActor::MarchCube(float InIsoLevel, const FIntVector& InPos)
{
	// 立方体の対応するキューブテーブル番号を求める
	int32 CubeIndex = 0;
	for(int32 I = 0; I < 8; ++I)
	{
		if(m_GridArray[m_GridRange.PosToIndex(InPos + MarchingCubesConsts::CubeVertices[I])] < InIsoLevel)
		{
			CubeIndex |= (1 << I);
		}
	}
	
	// 格子が完全に内部または外部にある場合はスルー
	int32 EdgeMask = MarchingCubesConsts::EdgeTable[CubeIndex];
	if(EdgeMask == 0)
	{
		return;
	}
	
	// 辺に対応する座標を求める
	static TArray<FVector> EdgeVertArray{0, 12};
	// 12個の辺それぞれのマスクを調べる
	for(int32 I = 0; I < 12; ++I)
	{
		// 交差座標の発生する辺かどうか
		if(EdgeMask & (1 << I))
		{
			// 対応する格子の座標番号を求める
			FIntVector PosIndex1 = InPos + MarchingCubesConsts::CubeVertices[MarchingCubesConsts::EdgeComb[I][0]];
			FIntVector PosIndex2 = InPos + MarchingCubesConsts::CubeVertices[MarchingCubesConsts::EdgeComb[I][1]];
			
			// 格子点の座標
			FVector P1 = FVector(PosIndex1) * m_CellSize;
			FVector P2 = FVector(PosIndex2) * m_CellSize;

			// 格子点の値
			float V1 = m_GridArray[m_GridRange.PosToIndex(PosIndex1)];
			float V2 = m_GridArray[m_GridRange.PosToIndex(PosIndex2)];

			// 補間した座標を求める
			EdgeVertArray[I] = InterpVertex(InIsoLevel, P1, P2, V1, V2);
		}
	}

	// TriTableを元に三角形の生成
	for(int32 I = 0; MarchingCubesConsts::TriTable[CubeIndex][I] != -1; I += 3)
	{
		int32 Idx1 = m_MeshData.m_Vertices.Emplace(EdgeVertArray[MarchingCubesConsts::TriTable[CubeIndex][I]]);
		int32 Idx2 = m_MeshData.m_Vertices.Emplace(EdgeVertArray[MarchingCubesConsts::TriTable[CubeIndex][I + 1]]);
		int32 Idx3 = m_MeshData.m_Vertices.Emplace(EdgeVertArray[MarchingCubesConsts::TriTable[CubeIndex][I + 2]]);
		m_MeshData.AddTriangle(Idx1, Idx2, Idx3);
	}
}

FVector AMarchingCubesMeshActor::InterpVertex(float InIsoLevel, const FVector& InP1, const FVector& InP2, float InV1, float InV2)
{
	float T = (InIsoLevel - InV1) / (InV2 - InV1);
	return InP1 + (InP2 - InP1) * T;
}

#if WITH_EDITOR

void AMarchingCubesMeshActor::GenerateMarchingCubesMeshEditor()
{
	GenerateMarchingCubesMesh();
}

#endif