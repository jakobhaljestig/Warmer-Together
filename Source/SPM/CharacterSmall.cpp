// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSmall.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"


void ACharacterSmall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(CrawlAction, ETriggerEvent::Started, this, &ACharacterSmall::Crawl);
		EnhancedInputComponent->BindAction(CrawlAction, ETriggerEvent::Completed, this, &ACharacterSmall::StopCrawl);
	
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

//Implementera check för att se om något är över karaktären. 

void ACharacterSmall::Crawl (const FInputActionValue& Value)
{
	UE_LOG(LogTemplateCharacter, Display, TEXT("Player Small is Crouching"));
	GetCharacterMovement()->MaxWalkSpeed = 250.0f;
	ACharacter::Crouch(true);
}

void ACharacterSmall::StopCrawl(const FInputActionValue& Value)
{
	UE_LOG(LogTemplateCharacter, Display, TEXT("Player Small is not Crouching"));
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	ACharacter::UnCrouch(true);
}

void ACharacterSmall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetVelocity().X < 50 && GetVelocity().Z > -50 && GetVelocity().Y < 50)
	{
		for (UActorComponent* Component : this->GetComponents())
		{
			if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(Component))
			{
				PrimComp->SetSimulatePhysics(false);
			}
		}
	}
}

