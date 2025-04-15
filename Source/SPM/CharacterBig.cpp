// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBig.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Pickup.h"
#include "InputActionValue.h"

UPickup* pickup;

void ACharacterBig::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// För test
		EnhancedInputComponent->BindAction(GrabAction, ETriggerEvent::Started, this, &ACharacterBig::ToggleGrab);
		EnhancedInputComponent->BindAction(ThrowAction, ETriggerEvent::Ongoing, this, &ACharacterBig::Throw);
	
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

//För test
void ACharacterBig::ToggleGrab (const FInputActionValue& Value)
{
	if (IsHolding)
	{
		UE_LOG(LogTemplateCharacter, Display, TEXT("Release triggered"));
		//pickup->Release();
		IsHolding = false;
	}else
	{
		UE_LOG(LogTemplateCharacter, Display, TEXT("Grab triggered"));
		//pickup->Grab();
		IsHolding = true;
	}
	
}

void ACharacterBig::Throw(const FInputActionValue& Value)
{
	if (IsHolding)
	{
		UE_LOG(LogTemplateCharacter, Display, TEXT("Throwing triggered"));
		IsHolding = false;
	}

}



