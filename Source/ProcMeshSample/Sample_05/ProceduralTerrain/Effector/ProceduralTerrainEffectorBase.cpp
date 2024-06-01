// Copyright seiroise.


#include "ProceduralTerrainEffectorBase.h"

FBox UProceduralTerrainEffectorBase::GetEffectRange() const
{
	return FBox{};
}

void UProceduralTerrainEffectorBase::Apply(URuntimeTerrainChunkObject* InChunk, float InDeltaTime)
{
}
