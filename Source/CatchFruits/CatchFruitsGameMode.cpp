// Copyright Epic Games, Inc. All Rights Reserved.

#include "CatchFruitsGameMode.h"
#include "CatchFruitsCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACatchFruitsGameMode::ACatchFruitsGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
