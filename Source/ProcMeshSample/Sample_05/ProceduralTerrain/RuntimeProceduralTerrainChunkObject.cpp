// Copyright seiroise.

#include "RuntimeProceduralTerrainChunkObject.h"

#include "ProceduralMeshComponent.h"
#include "ProceduralTerrainProfileDataAsset.h"
#include "ProcMeshSample/Sample_05/Polygonizer/PolygonizerInterface.h"

FVector URuntimeTerrainChunkObject::GetGridLocationAt(const FIntVector& InPosIndex) const
{
	if(!IsValid(m_pProfile))
	{
		return m_Origin;
	}

	return m_Origin + FVector(InPosIndex) * m_pProfile->m_CellSize;
}

float URuntimeTerrainChunkObject::GetGridValueAt(int32 InIndex) const
{
	return m_VolumeData[InIndex];
}

void URuntimeTerrainChunkObject::SetGridValueAt(int32 InIndex, float InNewValue)
{
	m_VolumeData[InIndex] = InNewValue;
}

void URuntimeTerrainChunkObject::OnVolumeDataUpdated(IPolygonizerInterface* InPolygonizer)
{
	if(InPolygonizer == nullptr)
	{
		return;
	}

	// メッシュを取得
	if(!IsValid(m_pActor))
	{
		return;
	}
	UProceduralMeshComponent* pMeshComp = m_pActor->GetComponentByClass<UProceduralMeshComponent>();
	if(!IsValid(pMeshComp))
	{
		return;
	}
	
	// メッシュを再生成する
	FPolygonizationInput Input;
	Input.m_IsoLevel = m_pProfile->m_IsoLevel;
	Input.m_VolumeData = m_VolumeData;
	Input.m_Grid = FIntVector{m_pProfile->m_ChunkSize};
	Input.m_SamplingInterval = m_pProfile->m_CellSize;

	// ポリゴン化処理
	InPolygonizer->Initialize(Input);
	InPolygonizer->Polygonize();

	// 出力を取得
	FPolygonizationOutput Output;
	InPolygonizer->GetOutput(Output);

	// メッシュを生成	
	Output.m_MeshData.CreateMeshSection(pMeshComp, 0, true);
}
