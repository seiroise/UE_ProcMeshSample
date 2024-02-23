// Copyright seiroise.

#include "MeshDataProxy.h"

#include "ProceduralMeshComponent.h"

void FMeshDataProxy::AddTriangle(int32 InV1, int32 InV2, int32 InV3)
{
	m_Triangles.Emplace(InV1);
	m_Triangles.Emplace(InV2);
	m_Triangles.Emplace(InV3);
}

void FMeshDataProxy::AddQuad(int32 InV1, int32 InV2, int32 InV3, int32 InV4)
{
	AddTriangle(InV1, InV2, InV3);
	AddTriangle(InV1, InV3, InV4);
}

void FMeshDataProxy::CreateMeshSection(UProceduralMeshComponent* InMeshComponent, int32 InMeshSection,
									   bool InCreateCollision)
{
	if (!IsValid(InMeshComponent))
	{
		return;
	}
	InMeshComponent->CreateMeshSection_LinearColor(
		InMeshSection,
		m_Vertices,
		m_Triangles,
		m_bUseNormals ? m_Normals : TArray<FVector>(),
		TArray<FVector2D>(),
		m_bUseColors ? m_Colors : TArray<FLinearColor>(),
		TArray<FProcMeshTangent>(),
		InCreateCollision
	);
}
