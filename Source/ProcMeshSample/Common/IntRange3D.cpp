#include "IntRange3D.h"

// =========================================================================================================
// FIntRange3D
// =========================================================================================================

FIntVector FIntRange3D::GetRandomPos(const FRandomStream& InRandomStream) const
{
	return FIntVector(
		InRandomStream.RandRange(m_Min.X, m_Max.X - 1),
		InRandomStream.RandRange(m_Min.Y, m_Max.Y - 1),
		InRandomStream.RandRange(m_Min.Z, m_Max.Z - 1)
	);
}

bool FIntRange3D::IsValidPos(const FIntVector& InPos) const
{
	return IsValidX(InPos.X) && IsValidY(InPos.Y) && IsValidZ(InPos.Z);
}

bool FIntRange3D::IsValidIndex(const int32 InIndex) const
{
	return InIndex >= 0 && InIndex < GetPosNum();
}

bool FIntRange3D::IsValidX(const int32 InX) const
{
	return InX >= m_Min.X && InX < m_Max.X;
}

bool FIntRange3D::IsValidY(const int32 InY) const
{
	return InY >= m_Min.Y && InY < m_Max.Y;
}

bool FIntRange3D::IsValidZ(const int32 InZ) const
{
	return InZ >= m_Min.Z && InZ < m_Max.Z;
}

int32 FIntRange3D::GetPosNum() const
{
	FIntVector Size = GetSize();
	return Size.X * Size.Y * Size.Z;
}

FIntVector FIntRange3D::GetPosAt(const int32 InIndex) const
{
	if (!IsValidIndex(InIndex))
	{
		return FIntVector(-1);
	}

	return IndexToPos(InIndex);
}

FIntVector FIntRange3D::GetLoopedPos(const FIntVector& InPos, bool InX, bool InY, bool InZ) const
{
	// 負の範囲については対象外
	return FIntVector(
		InX ? InPos.X % (m_Max.X - m_Min.X) + m_Min.X : InPos.X,
		InY ? InPos.Y % (m_Max.Y - m_Min.Y) + m_Min.Y : InPos.Y,
		InZ ? InPos.Z % (m_Max.Z - m_Min.Z) + m_Min.Z : InPos.Z
	);
}

FIntVector FIntRange3D::GetHorizontalLoopedPos(const FIntVector& InPos) const
{
	return GetLoopedPos(InPos, true, true, false);
}

FIntVector FIntRange3D::GetVerticalLoopedPos(const FIntVector& InPos) const
{
	return GetLoopedPos(InPos, false, true);
}

FIntVector FIntRange3D::GetSize() const
{
	return m_Max - m_Min;
}

FVector FIntRange3D::GetVolumeFloat() const
{
	return FVector((m_Max - FIntVector(1)) - m_Min);
}

FVector FIntRange3D::GetCenterFloat() const
{
	return FVector(m_Min) + GetVolumeFloat() * 0.5f;
}

int32 FIntRange3D::PosToIndex(const FIntVector& InPos) const
{
	if (!IsValidPos(InPos))
	{
		return -1;
	}

	FIntVector Local = InPos - m_Min;
	FIntVector Size = GetSize();
	return Local.Z * (Size.X * Size.Y) + Local.Y * Size.X + Local.X;
}

FIntVector FIntRange3D::IndexToPos(const int32 InIndex) const
{
	if (!IsValidIndex(InIndex))
	{
		return FIntVector(-1);
	}

	FIntVector Size = GetSize();
	int32 Z = InIndex / (Size.X * Size.Y);
	const int32 T = InIndex % (Size.X * Size.Y);
	int32 Y = T / Size.X;
	int32 X = T % Size.X;

	return FIntVector(X + m_Min.X, Y + m_Min.Y, Z + m_Min.Z);
}

FIntRange3D FIntRange3D::SliceX(const int32 InX) const
{
	if (!IsValidX(InX))
	{
		return FIntRange3D(0);
	}
	return FIntRange3D(FIntVector(InX, m_Min.Y, m_Min.Z), FIntVector(InX + 1, m_Max.Y, m_Max.Z));
}

FIntRange3D FIntRange3D::SliceY(const int32 InY) const
{
	if (!IsValidY(InY))
	{
		return FIntRange3D(0);
	}
	return FIntRange3D(FIntVector(m_Min.X, InY, m_Min.Z), FIntVector(m_Max.X, InY + 1, m_Max.Z));
}

FIntRange3D FIntRange3D::SliceZ(const int32 InZ) const
{
	if (!IsValidZ(InZ))
	{
		return FIntRange3D(0);
	}
	return FIntRange3D(FIntVector(m_Min.X, m_Min.Y, InZ), FIntVector(m_Max.X, m_Max.Y, InZ + 1));
}

FIntRange3D FIntRange3D::Expand(const int32 InSize) const
{
	return FIntRange3D(m_Min - FIntVector(InSize), m_Max + FIntVector(InSize));
}

FIntRange3D FIntRange3D::ExpandMin(const int32 InSize) const
{
	return FIntRange3D(m_Min - FIntVector{InSize}, m_Max);
}

FIntRange3D FIntRange3D::ExpandMax(const int32 InSize) const
{
	return FIntRange3D(m_Min, m_Max + FIntVector{InSize});
}

// =========================================================================================================
// FIntRange3DIterator
// =========================================================================================================

FIntRange3DIterator FIntRange3D::begin() const
{
	return FIntRange3DIterator(this, m_Min);
}

FIntRange3DIterator FIntRange3D::end() const
{
	// 終端要素はZ成分のみ(X, Yと繰り上がるため)
	return FIntRange3DIterator(this, FIntVector(m_Min.X, m_Min.Y, m_Max.Z));
}
