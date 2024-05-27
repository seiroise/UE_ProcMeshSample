// Copyright seiroise.


#include "ProceduralTerrainProfileDataAsset.h"

FIntVector UProceduralTerrainProfileDataAsset::GetChunkHashFromLocation(const FVector& InLocation) const
{
	FVector ChunkHashFloat = InLocation / ((m_ChunkSize - 1) * m_CellSize);
	return FIntVector{
		FMath::FloorToInt32(ChunkHashFloat.X), FMath::FloorToInt32(ChunkHashFloat.Y), FMath::FloorToInt32(ChunkHashFloat.Z)
	};
}
