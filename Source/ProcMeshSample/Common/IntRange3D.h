#pragma once

#include "CoreMinimal.h"
#include "IntRange3D.generated.h"

struct FIntRange3DIterator;

// =========================================================================================================
// 3次元空間の特定の範囲を表す構造体
// =========================================================================================================
USTRUCT()
struct FIntRange3D
{
	GENERATED_BODY()

public:
	FIntRange3D() = default;

	FIntRange3D(int32 InDimension)
		: m_Min(0), m_Max(InDimension)
	{
	}

	FIntRange3D(int32 InMinX, int32 InMinY, int32 InMaxX, int32 InMaxY)
		: m_Min(InMinX, InMinY, 0), m_Max(InMaxX, InMaxY, 1)
	{
	}

	FIntRange3D(FIntVector InMin, FIntVector InMax)
		: m_Min(InMin), m_Max(InMax)
	{
	}

	FIntRange3D(FIntVector2 InMin, FIntVector2 InMax)
		: m_Min(InMin.X, InMin.Y, 0), m_Max(InMax.X, InMax.Y, 1)
	{
	}

public:
	/**
	 * @brief 範囲内のランダムな座標に取得
	 * @param InRandomStream 
	 * @return 
	 */
	FIntVector GetRandomPos(const FRandomStream& InRandomStream) const;

	/**
	 * @brief 適切な座標かどうか。Minを含み、Maxを含まないような判定
	 * @param InPos 
	 * @return 
	 */
	bool IsValidPos(const FIntVector& InPos) const;

	/**
	 * @brief 適切な番号かどうか。
	 * @param InIndex 
	 * @return 
	 */
	bool IsValidIndex(const int32 InIndex) const;

	/**
	 * @brief 適切なX座標か
	 * @param InX 
	 * @return 
	 */
	bool IsValidX(const int32 InX) const;

	/**
	 * @brief 適切なY座標か
	 * @param InY 
	 * @return 
	 */
	bool IsValidY(const int32 InY) const;

	/**
	 * @brief 適切なZ座標か
	 * @param InZ 
	 * @return 
	 */
	bool IsValidZ(const int32 InZ) const;

	/**
	 * @brief 範囲に含まれる座標の数
	 * @return 
	 */
	int32 GetPosNum() const;

	/**
	 * @brief 番号に対応した座標を取得
	 * @param InIndex 
	 * @return 
	 */
	FIntVector GetPosAt(const int32 InIndex) const;

	/**
	 * @brief InPosを範囲内にLoopさせた座標を取得する。
	 * @param InPos 
	 * @return 
	 */
	FIntVector GetLoopedPos(const FIntVector& InPos, bool InX = true, bool InY = true, bool InZ = true) const;

	/**
	 * @brief InPosを水平方向にのみLoopさせた座標を取得する。
	 * @param InPos 
	 * @return 
	 */
	FIntVector GetHorizontalLoopedPos(const FIntVector& InPos) const;

	/**
	 * @brief InPosを垂直方向にのみLoopさせた座標を取得する。
	 * @param InPos 
	 * @return 
	 */
	FIntVector GetVerticalLoopedPos(const FIntVector& InPos) const;

	/**
	 * @brief MinからMaxの差を取得
	 * @return 
	 */
	FIntVector GetSize() const;

	/**
	 * MinからMaxの差を取得(FVectorで)
	 * @return 
	 */
	FVector GetSizeFloat() const;

	/**
	 * @brief 中心座標を取得(floatで)
	 * @return 
	 */
	FVector GetCenterFloat() const;

	/**
	 * @brief 3次元配列に見立てて、その座標の番号を取得する。
	 * @param InPos 
	 * @return 
	 */
	int32 PosToIndex(const FIntVector& InPos) const;

	/**
	 * @brief 3次元配列に見立てて、その番号から座標を取得する。
	 * @param InIndex 
	 * @return 
	 */
	FIntVector IndexToPos(const int32 InIndex) const;

	/**
	 * @brief 指定のX軸でスライスしたYZ平面の範囲を取得する
	 * @param InX 
	 * @return 
	 */
	FIntRange3D SliceX(const int32 InX) const;

	/**
	 * @brief 指定のY軸でスライスしたXZ平面の範囲を取得する
	 * @param InY 
	 * @return 
	 */
	FIntRange3D SliceY(const int32 InY) const;

	/**
	 * @brief 指定のZ軸でスライスしたXY平面の範囲を取得する
	 * @param InZ 
	 * @return 
	 */
	FIntRange3D SliceZ(const int32 InZ) const;

	/**
	 * @brief 指定のサイズだけmin, maxの両方向を拡張した範囲を取得
	 * @param InSize 
	 * @return 
	 */
	FIntRange3D Expand(const int32 InSize) const;

	/**
	 * 指定のサイズだけminを拡張した範囲を取得
	 * @param InSize 
	 * @return 
	 */
	FIntRange3D ExpandMin(const int32 InSize) const;

	/**
	 * 指定のサイズだけmaxを拡張した範囲を取得
	 * @param InSize 
	 * @return 
	 */
	FIntRange3D ExpandMax(const int32 InSize) const;

	// =========================================================================================================
	// C++イテレータ
	// =========================================================================================================
	FIntRange3DIterator begin() const;
	FIntRange3DIterator end() const;

public:
	/**
	 * @brief 領域の最小値
	 */
	UPROPERTY(EditAnywhere)
	FIntVector m_Min;

	/**
	 * @brief 領域の最大値
	 */
	UPROPERTY(EditAnywhere)
	FIntVector m_Max;
};

/**
 * @brief FIntRange3Dのイテレータ
 */
struct FIntRange3DIterator
{
public:
	FIntRange3DIterator() = default;

	explicit FIntRange3DIterator(const FIntRange3D* InRange, FIntVector InPos) : m_pRange(InRange),
		m_Pos(InPos)
	{
	}

	FIntRange3DIterator& operator++()
	{
		// X > Y > Zの順番に進めていく
		m_Pos.X++;
		if (m_Pos.X >= m_pRange->m_Max.X)
		{
			m_Pos.X = m_pRange->m_Min.X;
			m_Pos.Y++;
		}
		if (m_Pos.Y >= m_pRange->m_Max.Y)
		{
			m_Pos.Y = m_pRange->m_Min.Y;
			m_Pos.Z++;
		}
		return *this;
	}

	const FIntVector& operator*() const
	{
		return m_Pos;
	}

	bool operator !=(const FIntRange3DIterator& rhs) const
	{
		return m_Pos != rhs.m_Pos;
	}

private:
	const FIntRange3D* m_pRange;
	FIntVector m_Pos;
};
