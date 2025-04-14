// Copyright Epic Games, Inc. All Rights Reserved.

#include "SPMGameMode.h"
#include "SPMCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASPMGameMode::ASPMGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
