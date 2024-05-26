// Copyright seiroise.


#include "ProceduralTerrainInvokerComponent.h"

#include "ProceduralTerrainChunkManager.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UProceduralTerrainInvokerComponent::UProceduralTerrainInvokerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UProceduralTerrainInvokerComponent::BeginPlay()
{
	Super::BeginPlay();

	// InvokerとしてOwnerを登録
	AProceduralTerrainChunkManager* pTerrainChunkManager = Cast<AProceduralTerrainChunkManager>(
		UGameplayStatics::GetActorOfClass(this, AProceduralTerrainChunkManager::StaticClass())
	);

	if(IsValid(pTerrainChunkManager))
	{
		pTerrainChunkManager->RegisterInvoker(this);
	}
}

// Called every frame
void UProceduralTerrainInvokerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UProceduralTerrainInvokerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Ownerを登録解除
	AProceduralTerrainChunkManager* pTerrainChunkManager = Cast<AProceduralTerrainChunkManager>(
		UGameplayStatics::GetActorOfClass(this, AProceduralTerrainChunkManager::StaticClass())
	);

	if(IsValid(pTerrainChunkManager))
	{
		pTerrainChunkManager->UnregisterInvoker(this);
	}
}
