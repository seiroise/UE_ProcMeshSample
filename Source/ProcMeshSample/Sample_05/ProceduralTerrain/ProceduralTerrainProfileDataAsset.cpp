// Copyright seiroise.


#include "ProceduralTerrainProfileDataAsset.h"

FIntVector UProceduralTerrainProfileDataAsset::GetChunkAddressFromLocationFloor(const FVector& InLocation) const
{
	FVector ChunkAddressFloat = InLocation / ((m_ChunkSize - 1) * m_CellSize);
	return FIntVector{
		FMath::FloorToInt32(ChunkAddressFloat.X), FMath::FloorToInt32(ChunkAddressFloat.Y), FMath::FloorToInt32(ChunkAddressFloat.Z)
	};
}

FIntVector UProceduralTerrainProfileDataAsset::GetChunkAddressFromLocationRound(const FVector& InLocation) const
{
	FVector ChunkAddressFloat = InLocation / ((m_ChunkSize - 1) * m_CellSize);
	return FIntVector{
		FMath::RoundToInt32(ChunkAddressFloat.X), FMath::RoundToInt32(ChunkAddressFloat.Y), FMath::RoundToInt32(ChunkAddressFloat.Z)
	};
}
