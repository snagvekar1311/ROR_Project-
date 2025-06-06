// Copyright Epic Games, Inc. All Rights Reserved.

#include "MaritimeGameGameMode.h"
#include "MaritimeGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMaritimeGameGameMode::AMaritimeGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
