// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterPlayerController.h"
#include "Blueprint/UserWidget.h"

void ACharacterPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	HUD = CreateWidget(this, HUDClass);
    if (HUD)
    {
    	HUD->AddToPlayerScreen();
    }
}