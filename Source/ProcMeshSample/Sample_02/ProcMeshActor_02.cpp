// Copyright seiroise.

#include "ProcMeshActor_02.h"

#include "ProceduralMeshComponent.h"
#include "ProcMeshSample/Common/MeshDataProxy.h"
#include "ProcMeshSample/Common/FaceRenderMask.h"

AProcMeshActor_02::AProcMeshActor_02()
{
	PrimaryActorTick.bCanEverTick = true;

	m_pProcMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Procedural Mesh Component"));
	SetRootComponent(m_pProcMeshComponent);
}


void AProcMeshActor_02::BeginPlay()
{
	Super::BeginPlay();

	GenerateCube();	
}


void AProcMeshActor_02::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#if WITH_EDITOR

void AProcMeshActor_02::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	GenerateCube();
}

#endif

void AProcMeshActor_02::GenerateCube()
{
	if(!IsValid(m_pProcMeshComponent))
	{
		return;
	}

	FMeshDataProxy MeshData;

	float HalfExtent = 50.f;

	MeshData.m_Vertices.Emplace(FVector(-HalfExtent, -HalfExtent, -HalfExtent));
	MeshData.m_Vertices.Emplace(FVector( HalfExtent, -HalfExtent, -HalfExtent));
	MeshData.m_Vertices.Emplace(FVector(-HalfExtent,  HalfExtent, -HalfExtent));
	MeshData.m_Vertices.Emplace(FVector( HalfExtent,  HalfExtent, -HalfExtent));
	
	MeshData.m_Vertices.Emplace(FVector(-HalfExtent, -HalfExtent, HalfExtent));
	MeshData.m_Vertices.Emplace(FVector( HalfExtent, -HalfExtent, HalfExtent));
	MeshData.m_Vertices.Emplace(FVector(-HalfExtent,  HalfExtent, HalfExtent));
	MeshData.m_Vertices.Emplace(FVector( HalfExtent,  HalfExtent, HalfExtent));

	// +X
	if((static_cast<uint8>(m_RenderMask) & static_cast<uint8>(EFaceRenderMask::Forward)) > 0)
	{
		MeshData.AddQuad(1, 5, 7, 3);	
	}
	// -X
	if((static_cast<uint8>(m_RenderMask) & static_cast<uint8>(EFaceRenderMask::Backward)) > 0)
	{
		MeshData.AddQuad(0, 2, 6, 4);
	}
	// +Y
	if((static_cast<uint8>(m_RenderMask) & static_cast<uint8>(EFaceRenderMask::Right)) > 0)
	{
		MeshData.AddQuad(2, 3, 7, 6);
	}
	// -Y
	if((static_cast<uint8>(m_RenderMask) & static_cast<uint8>(EFaceRenderMask::Left)) > 0)
	{
		MeshData.AddQuad(0, 4, 5, 1);
	}
	// +Z
	if((static_cast<uint8>(m_RenderMask) & static_cast<uint8>(EFaceRenderMask::Top)) > 0)
	{
		MeshData.AddQuad(4, 6, 7, 5);
	}
	// -Z
	if((static_cast<uint8>(m_RenderMask) & static_cast<uint8>(EFaceRenderMask::Bottom)) > 0)
	{
		MeshData.AddQuad(0, 1, 3, 2);
	}

	MeshData.CreateMeshSection(m_pProcMeshComponent, 0, false);
}
