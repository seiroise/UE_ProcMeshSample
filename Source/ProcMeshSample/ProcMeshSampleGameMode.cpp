// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProcMeshSampleGameMode.h"
#include "ProcMeshSampleCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProcMeshSampleGameMode::AProcMeshSampleGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
