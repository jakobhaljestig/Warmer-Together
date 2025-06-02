// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterPlayerController.h"

#include "BodyTemperature.h"
#include "CharacterBase.h"
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

	if (Cast<ACharacterBase>(InPawn))
	{
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
	
		Cast<ACharacterBase>(InPawn)->GetBodyTemperature()->SetPlayerController(this);
	}
}

void ACharacterPlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	if (HUD != nullptr)
		HUD->RemoveFromParent();

	if (FreezeEffect != nullptr)
		FreezeEffect->RemoveFromParent();

	HUD = nullptr;
	FreezeEffect = nullptr;
}


