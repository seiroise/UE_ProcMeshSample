// Copyright seiroise.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProcMeshActor_01.generated.h"

class UProceduralMeshComponent;
/**
 * サンプル01, 三角形の作成
 */
UCLASS()
class PROCMESHSAMPLE_API AProcMeshActor_01 : public AActor
{
	GENERATED_BODY()

public:
	AProcMeshActor_01();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:

	void GenerateTriangle();
	
private:

	UPROPERTY()
	TObjectPtr<UProceduralMeshComponent> m_pProcMeshComponent;
};
