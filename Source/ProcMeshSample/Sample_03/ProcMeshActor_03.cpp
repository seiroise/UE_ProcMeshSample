// Copyright seiroise.

#include "ProcMeshActor_03.h"

#include "EnhancedInputComponent.h"
#include "ProceduralMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "ProcMeshSample/Common/MeshDataProxy.h"

AProcMeshActor_03::AProcMeshActor_03()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));

	m_pProcMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Procedural Mesh"));
	m_pProcMeshComponent->SetupAttachment(RootComponent);

	m_pCanvasNormalArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Canvas Normal"));
	m_pProcMeshComponent->SetupAttachment(RootComponent);
}

void AProcMeshActor_03::BeginPlay()
{
	Super::BeginPlay();

	// 入力を有効化
	EnableInput(GetWorld()->GetFirstPlayerController());
	// 各種入力受け取りのセットアップ
	if(UEnhancedInputComponent* InputComp = Cast<UEnhancedInputComponent>(InputComponent))
	{
		InputComp->BindAction(m_pMouseLeftButton, ETriggerEvent::Started, this, &AProcMeshActor_03::OnMouseLeftButton);
		InputComp->BindAction(m_pMouseLeftButton, ETriggerEvent::Triggered, this, &AProcMeshActor_03::OnMouseLeftButton);
		InputComp->BindAction(m_pMouseLeftButton, ETriggerEvent::Completed, this, &AProcMeshActor_03::OnMouseLeftButton);
		InputComp->BindAction(m_pMouseMovement, ETriggerEvent::Triggered, this, &AProcMeshActor_03::OnMouseMovement);
	}
}

void AProcMeshActor_03::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProcMeshActor_03::OnMouseLeftButton(const FInputActionInstance& InInstance)
{
	if(InInstance.GetTriggerEvent() == ETriggerEvent::Triggered)
	{
		// 線を描画できる & カーソルが自身に重なっている場合のみ
		if(m_bCanDraw)
		{
			FHitResult Hit;
			if(IsCursorOverlapping(Hit))
			{
				m_bIsDrawing = true;
				m_MeshSectionIndex++;
				m_LinePosArray.Reset();
			}	
		}
	}
	else if(InInstance.GetTriggerEvent() == ETriggerEvent::Completed)
	{
		m_bIsDrawing = false;
	}
}

void AProcMeshActor_03::OnMouseMovement(const FInputActionInstance& InInstance)
{
	if(!m_bIsDrawing)
	{
		return;
	}

	// 自身の上にカーソルがあることを確認
	FHitResult Hit;
	if(!IsCursorOverlapping(Hit))
	{
		return;
	}

	// 自身のローカル座標系に変換
	const FTransform& ActorTransform = GetActorTransform();
	FVector PosLocal = ActorTransform.InverseTransformPosition(Hit.Location);
	m_LinePosArray.Emplace(PosLocal);

	// メッシュを生成
	GenerateLine();
}

bool AProcMeshActor_03::IsCursorOverlapping(FHitResult& OutResult) const
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if(!IsValid(PC))
	{
		return false;
	}

	if(!PC->GetHitResultUnderCursor(ECC_Visibility, false, OutResult))
	{
		return false;
	}

	if(OutResult.GetActor() != this)
	{
		return false;
	}
	
	return true;
}

void AProcMeshActor_03::GenerateLine()
{
	if(m_LinePosArray.Num() <= 1)
	{
		return;
	}

	FMeshDataProxy MeshData;

	// 基準となる法線
	FVector CanvasNormal = m_pCanvasNormalArrowComponent->GetForwardVector();
	FVector DirNext = FVector::ZeroVector;

	// 面に対応する頂点を追加
	for(int32 I = 0, J = 1; J < m_LinePosArray.Num(); I = J++)
	{
		FVector Start = m_LinePosArray[I];
		FVector End = m_LinePosArray[J];

		FVector StartToEnd = End - Start;
		DirNext = StartToEnd.GetSafeNormal();

		// Startを中心、DirNextを法線とした平面上の点を頂点として追加
		AddLineSegmentPlaneVertices(MeshData, Start, CanvasNormal, DirNext, m_LineThickness, m_LineDepth);
	}

	// 最後の面に対応する頂点を追加
	{
		FVector Start = m_LinePosArray.Last();
		// Startを中心、DirNextを法線とした平面上の点を頂点として追加
		AddLineSegmentPlaneVertices(MeshData, Start, CanvasNormal, DirNext, m_LineThickness, m_LineDepth);
	}

	// インデックスを追加
	// 並行している4面
	for(int32 I = 0, J = 1; J < m_LinePosArray.Num(); I = J++)
	{
		int32 IndexStartPlane = I * 4;
		int32 IndexEndPlane = J * 4;

		// +side面
		MeshData.AddQuad(IndexStartPlane + 0, IndexStartPlane + 1, IndexEndPlane + 1, IndexEndPlane + 0);
		// -side面
		MeshData.AddQuad(IndexStartPlane + 3, IndexStartPlane + 2, IndexEndPlane + 2, IndexEndPlane + 3);
		// +normal面
		MeshData.AddQuad(IndexStartPlane + 2, IndexStartPlane + 0, IndexEndPlane + 0, IndexEndPlane + 2);
		// -normal面
		MeshData.AddQuad(IndexStartPlane + 1, IndexStartPlane + 3, IndexEndPlane + 3, IndexEndPlane + 1);
	}

	// 閉じるための2面
	{
		MeshData.AddQuad(0, 2, 3, 1);
		int32 IndexEndPlane = (m_LinePosArray.Num() - 1) * 4;
		MeshData.AddQuad(IndexEndPlane + 0, IndexEndPlane + 1, IndexEndPlane + 3, IndexEndPlane + 2);	
	}

	// メッシュを生成
	MeshData.CreateMeshSection(m_pProcMeshComponent, m_MeshSectionIndex);	
}

void AProcMeshActor_03::AddLineSegmentPlaneVertices(FMeshDataProxy& InMeshData, const FVector& InCenter,
	const FVector& InCanvasNormal, const FVector& InDirNext, float InThickness, float InDepth)
{
	FVector DirThickness = FVector::CrossProduct(InCanvasNormal, InDirNext);
	FVector V1 = InCenter + DirThickness * InThickness * 0.5f + InCanvasNormal * InDepth * 0.5f;
	FVector V2 = InCenter + DirThickness * InThickness * 0.5f - InCanvasNormal * InDepth * 0.5f;
	FVector V3 = InCenter - DirThickness * InThickness * 0.5f + InCanvasNormal * InDepth * 0.5f;
	FVector V4 = InCenter - DirThickness * InThickness * 0.5f - InCanvasNormal * InDepth * 0.5f;

	InMeshData.m_Vertices.Emplace(V1);
	InMeshData.m_Vertices.Emplace(V2);
	InMeshData.m_Vertices.Emplace(V3);
	InMeshData.m_Vertices.Emplace(V4);
}
