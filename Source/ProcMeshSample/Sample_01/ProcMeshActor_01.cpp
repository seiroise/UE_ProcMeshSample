// Copyright seiroise.


#include "ProcMeshActor_01.h"

#include "ProceduralMeshComponent.h"
#include "ProcMeshSample/Common/MeshDataProxy.h"

AProcMeshActor_01::AProcMeshActor_01()
{
	PrimaryActorTick.bCanEverTick = true;

	m_pProcMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Procedural Mesh Component"));
	SetRootComponent(m_pProcMeshComponent);
}

void AProcMeshActor_01::BeginPlay()
{
	Super::BeginPlay();

	GenerateTriangle();
}

void AProcMeshActor_01::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProcMeshActor_01::GenerateTriangle()
{
	if(!IsValid(m_pProcMeshComponent))
	{
		return;
	}

	FMeshDataProxy MeshData;

	MeshData.m_Vertices.Emplace(FVector(0.f, 0.f, 0.f));
	MeshData.m_Vertices.Emplace(FVector(100.f, 0.f, 0.f));
	MeshData.m_Vertices.Emplace(FVector(0.f, 0.f, 100.f));

	MeshData.AddTriangle(0, 1, 2);

	MeshData.CreateMeshSection(m_pProcMeshComponent, 0, false);
}
