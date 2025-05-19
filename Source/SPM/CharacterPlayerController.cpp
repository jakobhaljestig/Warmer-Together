// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterPlayerController.h"
#include "Blueprint/UserWidget.h"

void ACharacterPlayerController::HideHUD(bool bShouldHide)
{
	if (bShouldHide)
	{
		HUD->RemoveFromParent();
        FreezeEffect->RemoveFromParent();
	}
	else
	{
		HUD->AddToPlayerScreen(5);
		FreezeEffect->AddToPlayerScreen(2);
	}
}

void ACharacterPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	HUD = CreateWidget(this, HUDClass);
    if (HUD)
    {
    	HUD->AddToPlayerScreen(5);
    }

	FreezeEffect = CreateWidget(this, FreezeEffectClass);
	if (FreezeEffect)
	{
		FreezeEffect->AddToPlayerScreen(1);
	}
}


