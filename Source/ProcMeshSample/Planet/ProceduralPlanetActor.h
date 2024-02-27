// Copyright seiroise.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralPlanetActor.generated.h"

/**
 * 自動生成の惑星
 */
UCLASS()
class PROCMESHSAMPLE_API AProceduralPlanetActor : public AActor
{
	GENERATED_BODY()

public:
	AProceduralPlanetActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
