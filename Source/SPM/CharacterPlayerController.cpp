// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterPlayerController.h"
#include "Blueprint/UserWidget.h"

void ACharacterPlayerController::DisplayFreezeEffect(bool bShouldDisplay) const
{
	if (FreezeEffect)
	{
		if (bShouldDisplay)
		{
			UE_LOG(LogTemp, Display, TEXT("Displaying Freeze Effect"));
			FreezeEffect->AddToPlayerScreen();
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("No Freeze"));
			FreezeEffect->RemoveFromParent();
		}
	}
}


void ACharacterPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	HUD = CreateWidget(this, HUDClass);
    if (HUD)
    {
    	HUD->AddToPlayerScreen();
    }

	FreezeEffect = CreateWidget(this, FreezeEffectClass);
}
