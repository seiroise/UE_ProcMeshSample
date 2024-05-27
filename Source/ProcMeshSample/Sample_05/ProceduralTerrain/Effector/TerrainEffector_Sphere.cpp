// Copyright seiroise.


#include "TerrainEffector_Sphere.h"

#include "ProcMeshSample/Common/IntRange3D.h"
#include "ProcMeshSample/Sample_05/ProceduralTerrain/ProceduralTerrainProfileDataAsset.h"
#include "ProcMeshSample/Sample_05/ProceduralTerrain/RuntimeProceduralTerrainChunkObject.h"

FBox UTerrainEffector_Sphere::GetEffectRange() const
{
	FVector Min = m_Location - FVector(m_Radius);
	FVector Max = m_Location + FVector(m_Radius);
	return FBox{Min, Max};
}

void UTerrainEffector_Sphere::Apply(URuntimeTerrainChunkObject* InChunk)
{
	if(!IsValid(InChunk))
	{
		return;
	}

	bool bIsModified = false;
	FIntRange3D Range = {InChunk->m_pProfile->m_ChunkSize};
	for(const FIntVector& PosIndex : Range)
	{
		FVector Loc = InChunk->GetGridLocationAt(PosIndex);
		float DistSqrd = FVector::DistSquared(Loc, m_Location);
		if(DistSqrd < m_Radius * m_Radius)
		{
			int32 Index = Range.PosToIndex(PosIndex);
			// 範囲内
			float Dist = FMath::Sqrt(DistSqrd);
			// 距離が遠くなるにつれて効果を弱める
			float Effect = 1.f - (Dist / m_Radius);
			float GridValue = InChunk->GetGridValueAt(Index);
			InChunk->SetGridValueAt(Index, GridValue + Effect * m_EffectStrength);

			// 編集済みフラグ
			bIsModified |= true;
		}
	}

	if(bIsModified)
	{
		// 更新フラグを立てる
		InChunk->m_bIsDirty = true;
	}
}
