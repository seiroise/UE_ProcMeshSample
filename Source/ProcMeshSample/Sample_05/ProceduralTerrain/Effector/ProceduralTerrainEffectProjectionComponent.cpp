// Copyright seiroise.


#include "ProceduralTerrainEffectProjectionComponent.h"

#include "TerrainEffector_Sphere.h"
#include "Kismet/GameplayStatics.h"
#include "ProcMeshSample/Sample_05/ProceduralTerrain/ProceduralTerrainActor.h"


// Sets default values for this component's properties
UProceduralTerrainEffectProjectionComponent::UProceduralTerrainEffectProjectionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UProceduralTerrainEffectProjectionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	m_pPlayerController = UGameplayStatics::GetPlayerController(this, 0);
	m_pTerrain = Cast<AProceduralTerrainActor>(UGameplayStatics::GetActorOfClass(this, AProceduralTerrainActor::StaticClass()));
}


// Called every frame
void UProceduralTerrainEffectProjectionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(IsValid(m_pPlayerController) && IsValid(m_pTerrain))
	{
		if(m_pPlayerController->WasInputKeyJustPressed(EKeys::RightMouseButton))
		{
			// カメラの前方方向を取得
			FHitResult Hit;
			if(m_pPlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), false, Hit))
			{
				// エフェクターの作成
				UTerrainEffector_Sphere* pEffector = NewObject<UTerrainEffector_Sphere>(this);
				pEffector->m_Location = Hit.Location;
				pEffector->m_Radius = m_Radius;
				pEffector->m_EffectStrength = m_EffectStrength;

				// エフェクターの登録
				m_pTerrain->RegisterEffector(pEffector);
				m_pEffector = pEffector;
			}
		}
		else if(m_pPlayerController->WasInputKeyJustReleased(EKeys::RightMouseButton))
		{
			// エフェクターの適用解除
			if(IsValid(m_pEffector))
			{
				m_pTerrain->UnregisterEffector(m_pEffector);
				m_pEffector = nullptr;
			}
		}
	}
}