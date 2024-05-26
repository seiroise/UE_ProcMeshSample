// Copyright seiroise.


#include "ProceduralTerrainActor.h"

#include "ProceduralMeshComponent.h"

#include "ProcMeshSample/Common/IntRange3D.h"
#include "ProcMeshSample/Sample_04/Noise/SimpleNoiseFilter.h"
#include "ProcMeshSample/Sample_05/Polygonizer/MarchingCubesPolygonizer.h"
#include "ProcMeshSample/Sample_05/Polygonizer/PolygonizerInterface.h"

AProceduralTerrainActor::AProceduralTerrainActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

// Called when the game starts or when spawned
void AProceduralTerrainActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AProceduralTerrainActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProceduralTerrainActor::GenerateTerrainChunkAt(const FIntVector& InChunkID)
{
	if (m_TerrainChunkMap.Contains(InChunkID))
	{
		// すでにチャンクIDが登録済み
		return;
	}

	FTerrainChunkEntry Entry;

	FIntRange3D Range{m_ChunkSize};

	TArray<float> VolumeData;
	VolumeData.Init(0.f, Range.GetPosNum());

	// チャンク開始座標
	FVector StartPos = FVector{InChunkID} * ((m_ChunkSize - 1) * m_CellSize);

	// ノイズの値をボリュームデータとして生成
	for (const FIntVector& PosIdx : Range)
	{
		int32 Index = Range.PosToIndex(PosIdx);
		FVector NoiseSamplingPos = (StartPos + FVector(PosIdx) * m_CellSize) * m_NoiseSamplingScale;
		float V = m_pNoiseFilter->Evaluate(NoiseSamplingPos);
		// UE_LOG(LogTemp, Log, TEXT("%s = %f"), *NoiseSamplingPos.ToString(), V);
		VolumeData[Index] = V;
	}

	Entry.m_VolumeData = VolumeData;

	// マップに追加
	m_TerrainChunkMap.Emplace(InChunkID, Entry);

	// ProceduralMeshComponentを生成してメッシュを割当
	AActor* pActor = GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), StartPos, FRotator::ZeroRotator);
	UProceduralMeshComponent* pMeshComp = NewObject<UProceduralMeshComponent>(pActor);
	pMeshComp->RegisterComponent();
	pMeshComp->AttachToComponent(pActor->GetRootComponent(), {EAttachmentRule::SnapToTarget, true});
	pMeshComp->SetWorldLocation(StartPos);

	if (!IsValid(m_pPolygonizer))
	{
		m_pPolygonizer = NewObject<UMarchingCubesPolygonizer>(this);
	}

	FPolygonizationInput Input;
	Input.m_IsoLevel = m_IsoLevel;
	Input.m_VolumeData = VolumeData;
	Input.m_Grid = FIntVector{m_ChunkSize};
	Input.m_SamplingInterval = m_CellSize;
	m_pPolygonizer->Initialize(Input);
	m_pPolygonizer->Polygonize();

	// 出力を取得
	FPolygonizationOutput Output;
	m_pPolygonizer->GetOutput(Output);

	// メッシュを生成
	Output.m_MeshData.CreateMeshSection(pMeshComp, 0, false);
}

void AProceduralTerrainActor::GenerateTerrainChunkActorLocation(AActor* InActor, int32 InExtent)
{
	if (!IsValid(InActor))
	{
		return;
	}

	int32 Extent = FMath::Max(0, InExtent);

	FVector Location = InActor->GetActorLocation();
	// 基準チャンクIDに変換
	FVector ChunkIDFloat = Location / ((m_ChunkSize - 1) * m_CellSize);
	FIntVector BaseChunkID{
		FMath::RoundToInt32(ChunkIDFloat.X), FMath::RoundToInt32(ChunkIDFloat.Y), FMath::RoundToInt32(ChunkIDFloat.Z)
	};

	// 範囲指定
	FIntVector RangeMin = BaseChunkID - FIntVector{Extent};
	FIntVector RangeMax = BaseChunkID + FIntVector{Extent};

	FIntRange3D Range{RangeMin, RangeMax};
	for(const auto& ChunkID : Range)
	{
		GenerateTerrainChunkAt(ChunkID);
	}
}

#if WITH_EDITOR

void AProceduralTerrainActor::GenerateTerrainChunkEditor()
{
	FIntRange3D Range{m_ChunkIDEditorStart, m_ChunkIDEditorEnd};
	for (const auto& PosIdx : Range)
	{
		GenerateTerrainChunkAt(PosIdx);
	}
}

#endif
