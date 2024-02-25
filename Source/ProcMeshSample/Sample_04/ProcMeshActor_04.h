// Copyright seiroise.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProcMeshActor_04.generated.h"

class UProceduralMeshComponent;
class UPlanetGenerationSettingsDataAsset;

UENUM()
enum class ETerrainUpdateType
{
	/** 全ての面を更新 */
	All,
	/** 上方向の一面だけを更新　*/
	Up,
};


/**
 * サンプル04。地形の生成
 */
UCLASS()
class PROCMESHSAMPLE_API AProcMeshActor_04 : public AActor
{
	GENERATED_BODY()

public:
	AProcMeshActor_04();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;

	virtual bool ShouldTickIfViewportsOnly() const override;

private:
	
	/**
	 * NormalizedCubeによる球体メッシュの生成
	 */
	void GenerateSphereMesh_NormalizedCube();
	
private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProceduralMeshComponent> m_pProcMeshComponent;

	UPROPERTY(EditAnywhere)
	ETerrainUpdateType m_UpdateType = ETerrainUpdateType::All;
	
	UPROPERTY(EditAnywhere, meta=(UIMin = 2, UIMax=200))
	int32 m_Resolution = 3;
	
	UPROPERTY(EditAnywhere)
	bool m_bCreateCollision = false;

	UPROPERTY(EditAnywhere)
	bool m_bInParallel;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPlanetGenerationSettingsDataAsset> m_pGenSettings;
	
	UPROPERTY(EditAnywhere)
	bool m_bUseNormals;

	UPROPERTY(EditAnywhere)
	bool m_bUseColors;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterial> m_pMaterial;
	
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> m_pMaterialInstanceDynamic;

	UPROPERTY(EditAnywhere)
	bool m_bDrawNormals;

	UPROPERTY(EditAnywhere)
	bool m_bAnimation;

	UPROPERTY(EditAnywhere)
	float m_AnimationSpeed = 1.f;

#if WITH_EDITOR
	// =========================================================================================================
	// エディタ用
	// =========================================================================================================
public:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

private:

	FDelegateHandle OnGenerationSettingsChangedDelegateHandle;
	
#endif
};
