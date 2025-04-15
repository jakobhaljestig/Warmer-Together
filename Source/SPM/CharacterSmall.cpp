// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSmall.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"



void ACharacterSmall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(CrawlAction, ETriggerEvent::Started, this, &ACharacterSmall::Crouch);
		EnhancedInputComponent->BindAction(CrawlAction, ETriggerEvent::Completed, this, &ACharacterSmall::StopCrouch);
	
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void ACharacterSmall::Crouch (const FInputActionValue& Value)
{
	UE_LOG(LogTemplateCharacter, Display, TEXT("Player Small is Crouching"));
}

void ACharacterSmall::StopCrouch(const FInputActionValue& Value)
{
	UE_LOG(LogTemplateCharacter, Display, TEXT("Player Small is not Crouching"));
}
