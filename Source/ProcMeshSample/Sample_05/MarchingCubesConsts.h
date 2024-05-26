// Copyright seiroise.

#pragma once

#include "CoreMinimal.h"

/**
 * Marching Cubesに必要な各種定数
 */
class MarchingCubesConsts
{
public:

	inline static const int32 NumCubeVertices = 8;
	
	
	/**
	 * CubeIndexに対応した交差座標の発生する辺のテーブル
	 */
	static const int32 EdgeTable[256];

	/**
	 * CubeIndexに対応した交差座標の三角形インデックスのテーブル
	 */
	static const int32 TriTable[256][16];

	/**
	 * 格子の頂点座標
	 */
	static const FIntVector CubeVertices[8];

	/**
	 * 辺を構成する格子頂点の組み合わせ
	 */
	static const int32 EdgeComb[12][2];
};
