// Copyright seiroise.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProceduralTerrainInvokerComponent.generated.h"

/**
 * このコンポーネントがアタッチされているアクターはProceduralTerrainの生成基準になる
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROCMESHSAMPLE_API UProceduralTerrainInvokerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UProceduralTerrainInvokerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	int32 GetExtent() const { return m_Extent; }
	
private:

	UPROPERTY(EditAnywhere, meta=(ClampMin = 0))
	int32 m_Extent = 1;
};
