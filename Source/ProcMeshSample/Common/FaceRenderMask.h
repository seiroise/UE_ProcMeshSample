// Copyright seiroise.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FaceRenderMask.generated.h"

/**
 * 面に対応するフラグ
 */
UENUM()
enum class EFaceRenderMask : uint8
{
	All = 255,
	Forward = 1,
	Backward = 2,
	Right = 4,
	Left = 8,
	Top = 16,
	Bottom = 32,
};