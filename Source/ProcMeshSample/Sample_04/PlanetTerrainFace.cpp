// Copyright seiroise.

#include "PlanetTerrainFace.h"

#include "PlanetGenerationSettingsDataAsset.h"

void UPlanetTerrainFace::Initialize(int32 InResolution, const FVector& InLocalUp,
                                    UPlanetGenerationSettingsDataAsset* InGenSettings,
                                    const FFloatRange& InXRange, const FFloatRange& InYRange)
{
	m_Resolution = InResolution;
	m_LocalUp = InLocalUp.GetSafeNormal();
	m_pGenSettings = InGenSettings;
	m_XRange = InXRange;
	m_YRange = InYRange;

	// InLocalUpをもとに直交基底を作成
	// 直交基底の場合は要素の入れ替えで可能
	m_AxisA = FVector(InLocalUp.Z, InLocalUp.X, InLocalUp.Y);
	m_AxisB = FVector::CrossProduct(InLocalUp, m_AxisA);
}

FGenerateVertexDataResult UPlanetTerrainFace::GenerateVertexData()
{
	// 平面をm_Resolutionで分割
	constexpr float EdgeLength = 100.f;
	constexpr float HalfEdgeLength = EdgeLength * 0.5f;

	float XMin = m_XRange.GetLowerBoundValue() * EdgeLength - HalfEdgeLength;
	float XSectionLength = m_XRange.Size<float>() * EdgeLength / (m_Resolution - 1);
	float YMin = m_YRange.GetLowerBoundValue() * EdgeLength - HalfEdgeLength;
	float YSectionLength = m_YRange.Size<float>() * EdgeLength / (m_Resolution - 1);

	// float HalfLength = EdgeLength * 0.5f;
	// float SectionLength = EdgeLength / (m_Resolution - 1);

	int32 VertexNum = m_Resolution * m_Resolution;
	m_Elevations.SetNum(VertexNum);
	m_UnitNormals.SetNum(VertexNum);
	m_Vertices.SetNum(VertexNum);
	m_Normals.SetNum(VertexNum);

	// 正規化のために高さの最大値と最小値を計算しておく
	float MinElevation = TNumericLimits<float>::Max();
	float MaxElevation = TNumericLimits<float>::Min();

	// 頂点・法線を追加
	ParallelFor(
		VertexNum,
		[&](int32 InIdx)
		{
			int32 X = InIdx % m_Resolution;
			int32 Y = InIdx / m_Resolution;

			FVector Point =
				(X * XSectionLength + XMin) * m_AxisA +
				(Y * YSectionLength + YMin) * m_AxisB +
				HalfEdgeLength * m_LocalUp;

			// 正規化
			FVector UnitNormal = Point.GetSafeNormal();
			m_UnitNormals[InIdx] = UnitNormal;
			// 高さを取得
			FPlanetElevationInfo ElevationInfo = CalculatePlanetElevation(UnitNormal);
			// 高さ情報を保持
			m_Elevations[InIdx] = ElevationInfo;

			// 頂点位置を計算
			float OceanElevation = ElevationInfo.m_OceanElevation;
			// Elevation <= 0の場所は海。なのでクランプする
			FVector Vertex = UnitNormal * OceanElevation;
			m_Vertices[InIdx] = Vertex;

			// 法線の計算
			m_Normals[InIdx] = CalculatePlanetNormal(UnitNormal);

			// 正規化用
			MinElevation = FMath::Min(MinElevation, OceanElevation);
			MaxElevation = FMath::Max(MaxElevation, OceanElevation);
		}
	);

	FGenerateVertexDataResult Result;
	Result.m_MinElevation = MinElevation;
	Result.m_MaxElevation = MaxElevation;
	return Result;
}

void UPlanetTerrainFace::GenerateVertexColor()
{
	if(!IsValid(m_pGenSettings))
	{
		return;
	}
	
	int32 VertexNum = m_Resolution * m_Resolution;
	// Colorは内部で変換されるのでMaterial側で使用する値にはUVを使用する。
	m_Colors.SetNum(VertexNum);
	m_UV1s.SetNum(VertexNum);

	float ElevationRange = m_MaxElevation - m_MinElevation;

	ParallelFor(
		VertexNum,
		[&](int32 InIdx)
		{
			// 0.5を海面として、1が最も高いところになる。
			float Elevation = m_Elevations[InIdx].m_Elevation;
			float ElevationRatio = ((Elevation - m_MinElevation) / ElevationRange) + 0.5f;
			// float OceanDepth = FMath::Abs(FMath::Min(m_Elevations[InIdx].m_NoiseValue, 0.f));
			// float IsLand = m_Elevations[InIdx].m_NoiseValue >= 0.f ? 1.f : 0.f;
			float BiomeTextureCoordinate = m_pGenSettings->CalculateBiomeTextureCoordinate(m_UnitNormals[InIdx]);

			FLinearColor BiomeColor = m_pGenSettings->CalculatePlanetColor(m_UnitNormals[InIdx], ElevationRatio);
			m_Colors[InIdx] = FLinearColor(BiomeColor);
			m_UV1s[InIdx] = FVector2D(ElevationRatio, BiomeTextureCoordinate);
		}
	);
}

FMeshDataProxy UPlanetTerrainFace::ConstructMeshData()
{
	FMeshDataProxy MeshData;

	// Quadを構築
	int32 QuadNum = (m_Resolution - 1) * (m_Resolution - 1);
	MeshData.m_Triangles.SetNum(QuadNum * 2 * 3);
	ParallelFor(
		QuadNum,
		[&](int32 InIdx)
		{
			int32 A = InIdx % (m_Resolution - 1);
			int32 B = InIdx / (m_Resolution - 1);

			int32 Idx1 = B * m_Resolution + A;
			int32 Idx2 = (B + 1) * m_Resolution + A;

			int32 StartIdx = InIdx * 6;

			MeshData.m_Triangles[StartIdx + 0] = Idx1 + 1;
			MeshData.m_Triangles[StartIdx + 1] = Idx1;
			MeshData.m_Triangles[StartIdx + 2] = Idx2;

			MeshData.m_Triangles[StartIdx + 3] = Idx1 + 1;
			MeshData.m_Triangles[StartIdx + 4] = Idx2;
			MeshData.m_Triangles[StartIdx + 5] = Idx2 + 1;
		}
	);

	// 頂点情報を設定
	MeshData.m_Vertices = m_Vertices;
	MeshData.m_Normals = m_Normals;
	MeshData.m_Colors = m_Colors;
	MeshData.m_UV1s = m_UV1s;

	return MeshData;
}

void UPlanetTerrainFace::SetMinMaxElevation(float InMinElevation, float InMaxElevation)
{
	m_MinElevation = InMinElevation;
	m_MaxElevation = InMaxElevation;
}

FPlanetElevationInfo UPlanetTerrainFace::CalculatePlanetElevation(const FVector& InPointOnUnitSphere) const
{
	if (!IsValid(m_pGenSettings))
	{
		return FPlanetElevationInfo();
	}

	return m_pGenSettings->CalculatePlanetElevation(InPointOnUnitSphere);
}

FVector UPlanetTerrainFace::CalculatePlanetNormal(const FVector& InPointOnUnitSphere) const
{
	if (!IsValid(m_pGenSettings))
	{
		return InPointOnUnitSphere;
	}

	// 接空間を求める
	FVector Normal = InPointOnUnitSphere;
	FVector Tangent = FVector::CrossProduct(FVector::UpVector, Normal).GetSafeNormal();
	FVector Binormal = FVector::CrossProduct(Tangent, Normal);

	// 周辺の高さから勾配を求める
	const float EPS = 0.01f;
	FVector S1 = (Normal + Tangent * EPS).GetSafeNormal();
	FVector S2 = (Normal - Tangent * EPS).GetSafeNormal();
	FVector S3 = (Normal + Binormal * EPS).GetSafeNormal();
	FVector S4 = (Normal - Binormal * EPS).GetSafeNormal();

	FVector VS1 = CalculatePlanetElevation(S1).m_OceanElevation * S1;
	FVector VS2 = CalculatePlanetElevation(S2).m_OceanElevation * S2;
	FVector VS3 = CalculatePlanetElevation(S3).m_OceanElevation * S3;
	FVector VS4 = CalculatePlanetElevation(S4).m_OceanElevation * S4;

	// 勾配を求める
	FVector TangentPlanet = (VS1 - VS2);
	FVector BinormalPlanet = (VS3 - VS4);

	// 最後に外積を計算
	return FVector::CrossProduct(BinormalPlanet, TangentPlanet).GetSafeNormal();
}
