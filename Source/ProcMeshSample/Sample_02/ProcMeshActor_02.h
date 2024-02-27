// Copyright seiroise.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProcMeshActor_02.generated.h"

enum class EFaceRenderMask : uint8;
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

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
private:

	void GenerateCube();
	
private:

	UPROPERTY(EditAnywhere)
	EFaceRenderMask m_RenderMask;

	UPROPERTY()
	TObjectPtr<UProceduralMeshComponent> m_pProcMeshComponent;
};
