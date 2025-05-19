
// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBig.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "LiftComponent.h"
#include "InputActionValue.h"

void ACharacterBig::BeginPlay()
{
	Super::BeginPlay();
	PickupComponent = FindComponentByClass<ULiftComponent>();
	if (!PickupComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("PickupComponent not valid"));
	}

	ClimbingComponent = FindComponentByClass<UClimbComponent>();
	if (!ClimbingComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Climbing component not valid"));
	}
}

void ACharacterBig::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		EnhancedInputComponent->BindAction(GrabAction, ETriggerEvent::Started, this, &ACharacterBig::ToggleGrab);

		EnhancedInputComponent->BindAction(ClimbAction, ETriggerEvent::Started, this, &ACharacterBig::Climb);
	
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ACharacterBig::ToggleGrab (const FInputActionValue& Value)
{
	if (!bIsClimbing && !bIsHugging && !)
	{
		PickupComponent->GrabAndRelease();
	}
}

void ACharacterBig::Climb(const FInputActionValue& Value)
{
	if (PickupComponent -> HoldingSomething() == false)
	{
		ClimbingComponent->Climb();
		bIsClimbing = ClimbingComponent->IsClimbing();
	}
	
}

void ACharacterBig::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		if (ClimbingComponent && ClimbingComponent->IsClimbing())
		{
			if (!ClimbingComponent->IsOnLedge() || MovementVector.Y < 0.f)
			{
				FVector ClimbDirection = FVector::UpVector;
				AddMovementInput(ClimbDirection, MovementVector.Y);
			}
			return;
		}
		Super::Move(Value);
	}
}

void ACharacterBig::OnDeath()
{
	PickupComponent->Drop(0,0);
	ClimbingComponent->StopClimb();
	Super::OnDeath();
}
