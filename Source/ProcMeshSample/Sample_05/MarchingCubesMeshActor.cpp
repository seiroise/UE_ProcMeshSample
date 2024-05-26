// Copyright seiroise.


#include "MarchingCubesMeshActor.h"

#include "MarchingCubesConsts.h"
#include "ProceduralMeshComponent.h"
#include "Polygonizer/MarchingCubesPolygonizer.h"
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

void AMarchingCubesMeshActor::GenerateMarchingCubesMesh()
{
	if (!IsValid(m_pPolygonizer))
	{
		m_pPolygonizer = NewObject<UMarchingCubesPolygonizer>(this);
	}

	// ボリュームデータを作成
	TArray<float> VolumeData;
	GenerateSphereVolumeData(m_GridCount, VolumeData);

	// 入力データを作成
	FPolygonizationInput Input;
	Input.m_IsoLevel = m_IsoLevel;
	Input.m_Grid = FIntVector(m_GridCount);
	Input.m_SamplingInterval = m_CellSize;
	Input.m_VolumeData = VolumeData;

	m_pPolygonizer->Initialize(Input);
	m_pPolygonizer->Polygonize();

	// 出力を取得
	FPolygonizationOutput Output;
	m_pPolygonizer->GetOutput(Output);

	// メッシュを生成
	Output.m_MeshData.CreateMeshSection(m_pProcMeshComponent, 0, false);
}

void AMarchingCubesMeshActor::GenerateSphereVolumeData(int32 InSize, TArray<float>& OutVolumeData)
{
	OutVolumeData.Reset();
	FIntRange3D Range = {InSize};

	FVector Center = Range.GetCenterFloat();
	float Radius = InSize * 0.5f;

	for (const FIntVector& Pos : Range)
	{
		// 範囲の中心の球体として、そこからの距離で初期化
		float Distance = (FVector(Pos) - Center).Length() / Radius;
		OutVolumeData.Emplace(Distance);
	}
}

#if WITH_EDITOR

void AMarchingCubesMeshActor::GenerateMarchingCubesMeshEditor()
{
	GenerateMarchingCubesMesh();
}

#endif
