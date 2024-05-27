// Copyright seiroise.


#include "ProceduralTerrainActor.h"

#include "ProceduralMeshComponent.h"
#include "ProceduralTerrainProfileDataAsset.h"
#include "RuntimeProceduralTerrainChunkObject.h"
#include "Effector/ProceduralTerrainEffectorBase.h"

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

	// エフェクターの適用
	ApplyEffectors(DeltaTime);
	
	// チャンクの更新
	UpdateDirtyChunks(DeltaTime);
}

void AProceduralTerrainActor::GenerateTerrainChunkAt(const FIntVector& InChunkHash)
{
	if(!IsValid(m_pProfile))
	{
		return;
	}
	
	if (m_TerrainChunkMap.Contains(InChunkHash))
	{
		// すでにチャンクIDが登録済み
		return;
	}

	FIntRange3D Range{m_pProfile->m_ChunkSize};

	TArray<float> VolumeData;
	VolumeData.Init(0.f, Range.GetPosNum());

	// チャンク開始座標
	FVector ChunkOrigin = FVector{InChunkHash} * ((m_pProfile->m_ChunkSize - 1) * m_pProfile->m_CellSize);

	// ノイズの値をボリュームデータとして生成
	for (const FIntVector& PosIdx : Range)
	{
		int32 Index = Range.PosToIndex(PosIdx);
		FVector NoiseSamplingPos = (ChunkOrigin + FVector(PosIdx) * m_pProfile->m_CellSize) * m_pProfile->m_NoiseSamplingScale;
		float V = m_pProfile->m_pNoiseFilter->Evaluate(NoiseSamplingPos);
		// UE_LOG(LogTemp, Log, TEXT("%s = %f"), *NoiseSamplingPos.ToString(), V);
		VolumeData[Index] = V;
	}

	// ProceduralMeshComponentを生成してメッシュを割当
	AActor* pActor = GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), ChunkOrigin, FRotator::ZeroRotator);
	UProceduralMeshComponent* pMeshComp = NewObject<UProceduralMeshComponent>(pActor);
	pMeshComp->RegisterComponent();
	pMeshComp->AttachToComponent(pActor->GetRootComponent(), {EAttachmentRule::SnapToTarget, true});
	pMeshComp->SetWorldLocation(ChunkOrigin);

	if (!IsValid(m_pPolygonizer))
	{
		m_pPolygonizer = NewObject<UMarchingCubesPolygonizer>(this);
	}

	FPolygonizationInput Input;
	Input.m_IsoLevel = m_pProfile->m_IsoLevel;
	Input.m_VolumeData = VolumeData;
	Input.m_Grid = FIntVector{m_pProfile->m_ChunkSize};
	Input.m_SamplingInterval = m_pProfile->m_CellSize;
	m_pPolygonizer->Initialize(Input);
	m_pPolygonizer->Polygonize();

	// 出力を取得
	FPolygonizationOutput Output;
	m_pPolygonizer->GetOutput(Output);

	// メッシュを生成
	Output.m_MeshData.CreateMeshSection(pMeshComp, 0, true);

	// チャンクデータを生成
	URuntimeTerrainChunkObject* pTerrainChunk = NewObject<URuntimeTerrainChunkObject>(this);
	pTerrainChunk->m_pProfile = m_pProfile;
	pTerrainChunk->m_Origin = ChunkOrigin;
	pTerrainChunk->m_VolumeData = VolumeData;
	pTerrainChunk->m_pActor = pActor;
	m_TerrainChunkMap.Emplace(InChunkHash, pTerrainChunk);
}

void AProceduralTerrainActor::GenerateTerrainChunkActorLocation(AActor* InActor, int32 InExtent)
{
	if(!IsValid(m_pProfile) || !IsValid(InActor))
	{
		return;
	}

	int32 Extent = FMath::Max(0, InExtent);

	FVector Location = InActor->GetActorLocation();
	FIntVector BaseChunkHash = m_pProfile->GetChunkHashFromLocation(Location);

	// 範囲指定
	FIntVector RangeMin = BaseChunkHash - FIntVector{Extent};
	FIntVector RangeMax = BaseChunkHash + FIntVector{Extent};

	FIntRange3D Range{RangeMin, RangeMax};
	for(const auto& ChunkHash : Range)
	{
		GenerateTerrainChunkAt(ChunkHash);
	}
}

void AProceduralTerrainActor::RegisterEffector(UProceduralTerrainEffectorBase* InEffector)
{
	if(!IsValid(InEffector))
	{
		return;
	}

	// 登録済み
	if(m_EffectorArray.Contains(InEffector))
	{
		return;
	}
	
	m_EffectorArray.Emplace(InEffector);
}

void AProceduralTerrainActor::UnregisterEffector(UProceduralTerrainEffectorBase* InEffector)
{
	if(!IsValid(InEffector))
	{
		return;
	}
	
	m_EffectorArray.Remove(InEffector);
}

void AProceduralTerrainActor::ApplyEffector(UProceduralTerrainEffectorBase* InEffector)
{
	if(!IsValid(m_pProfile) || !IsValid(InEffector))
	{
		return;
	}

	// 効果範囲から対応するチャンクの範囲を求める
	FBox EffectRange = InEffector->GetEffectRange();

	FIntVector ChunkHashMin = m_pProfile->GetChunkHashFromLocation(EffectRange.Min);
	FIntVector ChunkHashMax = m_pProfile->GetChunkHashFromLocation(EffectRange.Max);

	FIntRange3D Range{ChunkHashMin, ChunkHashMax};
	// 反復用に範囲を１つ広げる
	Range = Range.ExpandMax(1);
	
	for(const FIntVector& ChunkHash : Range)
	{
		URuntimeTerrainChunkObject** ppChunk = m_TerrainChunkMap.Find(ChunkHash);
		if(ppChunk != nullptr && IsValid(*ppChunk))
		{
			URuntimeTerrainChunkObject* pChunk = *ppChunk;
			// 効果を適用
			InEffector->Apply(pChunk);
			// チャンクの更新フラグを確認
			if(pChunk->m_bIsDirty)
			{
				// 更新が必要なチャンクの配列に追加
				m_DirtyChunkArray.Emplace(pChunk);
			}
		}
	}
}

void AProceduralTerrainActor::ApplyEffectors(float InDeltaTime)
{
	// 登録されているエフェクターを順次適用
	for(UProceduralTerrainEffectorBase* pEffector : m_EffectorArray)
	{
		ApplyEffector(pEffector);
	}
}

void AProceduralTerrainActor::UpdateDirtyChunks(float InDeltaTime)
{
	for(URuntimeTerrainChunkObject* pChunk : m_DirtyChunkArray)
	{
		if(!IsValid(pChunk))
		{
			continue;
		}

		// ボリュームデータの更新
		pChunk->OnVolumeDataUpdated(m_pPolygonizer);

		// Dirtyフラグを下ろす
		pChunk->m_bIsDirty = false;
	}

	// 配列は初期化
	m_DirtyChunkArray.Reset();
}

#if WITH_EDITOR

void AProceduralTerrainActor::GenerateTerrainChunkEditor()
{
	FIntRange3D Range{m_ChunkHashEditorStart, m_ChunkHashEditorEnd};
	for (const auto& PosIdx : Range)
	{
		GenerateTerrainChunkAt(PosIdx);
	}
}

#endif
