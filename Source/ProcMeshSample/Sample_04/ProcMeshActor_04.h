// Copyright seiroise.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProcMeshActor_04.generated.h"

UCLASS()
class PROCMESHSAMPLE_API AProcMeshActor_04 : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProcMeshActor_04();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
