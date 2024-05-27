// Copyright seiroise.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ProceduralTerrainEffectProjectionComponent.generated.h"

class UProceduralTerrainEffectorBase;
class AProceduralTerrainActor;

/**
 * 地形にEffectorを適用するためのコンポーネント
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROCMESHSAMPLE_API UProceduralTerrainEffectProjectionComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UProceduralTerrainEffectProjectionComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	/**
	 * エフェクト半径
	 */
	UPROPERTY(EditAnywhere)
	float m_Radius = 200.f;

	/**
	 * エフェクトの強さ
	 */
	UPROPERTY(EditAnywhere)
	float m_EffectStrength = 1.f;

	/**
	 * プレイヤーコントローラー(入力用)
	 */
	UPROPERTY()
	TObjectPtr<APlayerController> m_pPlayerController;
	
	/**
	 * 地形用アクター
	 */
	UPROPERTY()
	TObjectPtr<AProceduralTerrainActor> m_pTerrain;

	/**
	 * 適用しているエフェクター
	 */
	UPROPERTY()
	TObjectPtr<UProceduralTerrainEffectorBase> m_pEffector;
};
