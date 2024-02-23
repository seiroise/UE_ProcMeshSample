// Copyright seiroise.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProcMeshActor_02.generated.h"

class UProceduralMeshComponent;

/**
 * サンプル02。立方体の作成
 */
UCLASS()
class PROCMESHSAMPLE_API AProcMeshActor_02 : public AActor
{
	GENERATED_BODY()

public:
	AProcMeshActor_02();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:

	void GenerateCube();
	
private:

	UPROPERTY()
	TObjectPtr<UProceduralMeshComponent> m_pProcMeshComponent;
};
