// Copyright seiroise.


#include "ProceduralTerrainChunkManager.h"

#include "ProceduralTerrainActor.h"
#include "ProceduralTerrainInvokerComponent.h"


// Sets default values
AProceduralTerrainChunkManager::AProceduralTerrainChunkManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AProceduralTerrainChunkManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProceduralTerrainChunkManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!m_InvokerArray.IsEmpty() && IsValid(m_pTerrainActor))
	{
		for(UProceduralTerrainInvokerComponent* pInvoker : m_InvokerArray)
		{
			// Invokerの存在するチャンクを生成する
			m_pTerrainActor->GenerateTerrainChunkActorLocation(pInvoker->GetOwner(), pInvoker->GetExtent());	
		}
	}
}

void AProceduralTerrainChunkManager::RegisterInvoker(UProceduralTerrainInvokerComponent* InInvoker)
{
	if(!IsValid(InInvoker))
	{
		return;
	}
	
	if(m_InvokerArray.Contains(InInvoker))
	{
		//すでに登録済み
		return;
	}

	m_InvokerArray.Emplace(InInvoker);
}

void AProceduralTerrainChunkManager::UnregisterInvoker(UProceduralTerrainInvokerComponent* InInvoker)
{
	if(!IsValid(InInvoker))
	{
		return;
	}
	
	if(!m_InvokerArray.Contains(InInvoker))
	{
		// 未登録
		return;
	}

	m_InvokerArray.Remove(InInvoker);
}

