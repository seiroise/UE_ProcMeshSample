// Copyright seiroise.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProcMeshActor_03.generated.h"

struct FMeshDataProxy;
struct FInputActionInstance;
class UProceduralMeshComponent;
class UArrowComponent;
class UInputAction;

/**
 * サンプル03。線の作成
 */
UCLASS()
class PROCMESHSAMPLE_API AProcMeshActor_03 : public AActor
{
	GENERATED_BODY()

public:
	AProcMeshActor_03();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetCanDraw(bool InValue) { m_bCanDraw = InValue; }
	
private:

	void OnMouseLeftButton(const FInputActionInstance& InInstance);
	void OnMouseMovement(const FInputActionInstance& InInstance);

	/**
	 * カーソルが自身と重なっているか
	 * @return 
	 */
	bool IsCursorOverlapping(FHitResult& OutResult) const;

	/**
	 * 線のメッシュを生成
	 */
	void GenerateLine();

	/**
	 * InDirNextを法線とする平面に対して、InCenterを中心としてInThicknessとInDepthだけ移動した平面上の4点を頂点として追加する。
	 * @param InMeshData 
	 * @param InCenter 
	 * @param InCanvasNormal 
	 * @param InDirNext 
	 * @param InThickness 
	 * @param InDepth 
	 */
	void AddLineSegmentPlaneVertices(FMeshDataProxy& InMeshData, const FVector& InCenter, const FVector& InCanvasNormal, const FVector& InDirNext, float InThickness, float InDepth);
	
private:

	UPROPERTY()
	TObjectPtr<UProceduralMeshComponent> m_pProcMeshComponent;

	/**
	 * キャンバスの法線方向
	 */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UArrowComponent> m_pCanvasNormalArrowComponent;
	
	/**
	 * マウス左ボタン
	 */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> m_pMouseLeftButton;

	/**
	 * マウス移動量
	 */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> m_pMouseMovement;

	/**
	 * 線の太さ
	 */
	UPROPERTY(EditAnywhere)
	float m_LineThickness = 10.f;

	/**
	 * 線の奥行き
	 */
	UPROPERTY(EditAnywhere)
	float m_LineDepth = 100.f;

	/**
	 * 線を描画することができるか。
	 */
	UPROPERTY(VisibleAnywhere)
	bool m_bCanDraw;

	/**
	 * マウス押し込み中
	 */
	UPROPERTY(VisibleAnywhere)
	bool m_bIsDrawing;

	/**
	 * 線を構成する頂点座標
	 */
	TArray<FVector> m_LinePosArray;
};
