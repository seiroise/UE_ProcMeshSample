// Copyright seiroise.

#include "ProcMeshActor_04.h"

#include "PlanetGenerationSettingsDataAsset.h"
#include "PlanetTerrainFace.h"
#include "ProceduralMeshComponent.h"
#include "ProcMeshSample/Common/ScopedTimer.h"

AProcMeshActor_04::AProcMeshActor_04()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	SetRootComponent(RootComponent);

	m_pProcMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Face_Up"));
	m_pProcMeshComponent->SetupAttachment(GetRootComponent());
}

void AProcMeshActor_04::BeginPlay()
{
	Super::BeginPlay();
	GenerateSphereMesh_NormalizedCube();
}

void AProcMeshActor_04::TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);

	if (TickType == LEVELTICK_ViewportsOnly)
	{
		// 確認用の法線の描画
		if (m_bDrawNormals)
		{
			if (IsValid(m_pProcMeshComponent))
			{
				int32 NumSections = m_pProcMeshComponent->GetNumSections();
				for (int32 I = 0; I < NumSections; ++I)
				{
					FProcMeshSection* pMeshSection = m_pProcMeshComponent->GetProcMeshSection(I);
					if (pMeshSection == nullptr)
					{
						continue;
					}

					const TArray<FProcMeshVertex>& VBuffer = pMeshSection->ProcVertexBuffer;
					for (const FProcMeshVertex& Vertex : VBuffer)
					{
						FVector Normal = Vertex.Normal;
						// FVector Tangent = FVector::CrossProduct(Normal, FVector::UpVector).GetSafeNormal();
						// FVector Binormal = FVector::CrossProduct(Tangent, Normal);
						DrawDebugLine(GetWorld(), Vertex.Position, Vertex.Position + Normal * 100.f, FColor::Red);
						// DrawDebugLine(GetWorld(),Vertex.Position,Vertex.Position + Tangent * 100.f,FColor::Green);
						// DrawDebugLine(GetWorld(),Vertex.Position,Vertex.Position + Binormal * 100.f,FColor::Blue);
					}
				}
			}
		}
	}
}

bool AProcMeshActor_04::ShouldTickIfViewportsOnly() const
{
	return true;
}

#if WITH_EDITOR

void AProcMeshActor_04::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	GenerateSphereMesh_NormalizedCube();

	if (IsValid(m_pGenSettings) && !m_pGenSettings->OnChanged.IsBoundToObject(this))
	{
		m_pGenSettings->OnChanged.AddUObject(this, &AProcMeshActor_04::GenerateSphereMesh_NormalizedCube);
	}
}

#endif

void AProcMeshActor_04::GenerateSphereMesh_NormalizedCube()
{
	if (!IsValid(m_pProcMeshComponent))
	{
		return;
	}

	FScopedTimer Scope(TEXT("Generate Planet:"));

	TArray<FVector> FaceVectors;

	// 指定の面のみ更新
	switch (m_RenderMask)
	{
	case EFaceRenderMask::All:
		FaceVectors.Emplace(FVector::ForwardVector);
		FaceVectors.Emplace(-FVector::ForwardVector);
		FaceVectors.Emplace(FVector::RightVector);
		FaceVectors.Emplace(-FVector::RightVector);
		FaceVectors.Emplace(FVector::UpVector);
		FaceVectors.Emplace(-FVector::UpVector);
		break;
	case EFaceRenderMask::Forward:
		FaceVectors.Emplace(FVector::ForwardVector);
		break;
	case EFaceRenderMask::Backward:
		FaceVectors.Emplace(FVector::BackwardVector);
		break;
	case EFaceRenderMask::Left:
		FaceVectors.Emplace(FVector::LeftVector);
		break;
	case EFaceRenderMask::Right:
		FaceVectors.Emplace(FVector::RightVector);
		break;
	case EFaceRenderMask::Top:
		FaceVectors.Emplace(FVector::UpVector);
		break;
	case EFaceRenderMask::Bottom:
		FaceVectors.Emplace(FVector::DownVector);
		break;
	}

	// メッシュ情報を削除
	m_pProcMeshComponent->ClearAllMeshSections();

	// 標高を正規化する必要があるので、最初に全体の最小/最大の標高を計算
	TArray<UPlanetTerrainFace*> TerrainFaceArray;
	float MinElevation = TNumericLimits<float>::Max();
	float MaxElevation = TNumericLimits<float>::Min();
	
	for (int32 I = 0; I < FaceVectors.Num(); ++I)
	{
		const FVector& LocalUpVector = FaceVectors[I];
		UPlanetTerrainFace* TerrainFace = NewObject<UPlanetTerrainFace>();
		TerrainFaceArray.Emplace(TerrainFace);
		
		TerrainFace->Initialize(m_Resolution, LocalUpVector, m_pGenSettings, FFloatRange(0.f, 1.f), FFloatRange(0.f, 1.f));

		// 頂点データを生成
		FGenerateVertexDataResult Result;
		{
			FScopedTimer Scope_A(FString::Printf(TEXT("Face [%i] generate vertices:"), I));
			Result = TerrainFace->GenerateVertexData();
		}

		MinElevation = FMath::Min(Result.m_MinElevation, MinElevation);
		MaxElevation = FMath::Max(Result.m_MaxElevation, MaxElevation);
	}

	// 標高データから正規化した色情報を生成
	for (int32 I = 0; I < FaceVectors.Num(); ++I)
	{
		const FVector& LocalUpVector = FaceVectors[I];
		UPlanetTerrainFace* TerrainFace = TerrainFaceArray[I];
		TerrainFace->SetMinMaxElevation(MinElevation, MaxElevation);
		
		// 色情報を生成
		{
			FScopedTimer Scope_B(FString::Printf(TEXT("Face [%i] generate colors:"), I));
			TerrainFace->GenerateVertexColor();	
		}
	}

	// 各種設定
	for (int32 I = 0; I < FaceVectors.Num(); ++I)
	{
		// メッシュ情報を生成
		UPlanetTerrainFace* TerrainFace = TerrainFaceArray[I];
		FMeshDataProxy MeshData;
		MeshData = TerrainFace->ConstructMeshData();
		MeshData.m_bUseNormals = m_bUseNormals;
		MeshData.m_bUseColors = m_bUseColors;
		MeshData.m_bUseUV1 = m_bUseColors;

		// メッシュセクション生成
		{
			FScopedTimer Scope_C(FString::Printf(TEXT("Face [%i] mesh creating"), I));
			MeshData.CreateMeshSection(m_pProcMeshComponent, I, m_bCreateCollision);	
		}
		
		// マテリアルを設定
		m_pProcMeshComponent->SetMaterial(I, m_pMaterial);
	}
}