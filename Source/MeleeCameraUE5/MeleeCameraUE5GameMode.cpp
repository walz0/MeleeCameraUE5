// Copyright Epic Games, Inc. All Rights Reserved.

#include "MeleeCameraUE5GameMode.h"
#include "MeleeCameraUE5Character.h"
#include "UObject/ConstructorHelpers.h"

AMeleeCameraUE5GameMode::AMeleeCameraUE5GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
