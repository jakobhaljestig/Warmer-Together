
// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBig.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "LiftComponent.h"
#include "InputActionValue.h"

ACharacterBig::ACharacterBig()
{
	LiftComponent = CreateDefaultSubobject<ULiftComponent>(TEXT("LiftComponent"));
	ClimbComponent = CreateDefaultSubobject<UClimbComponent>(TEXT("ClimbingComponent"));
}

void ACharacterBig::ResetPlayerState()
{
	Super::ResetPlayerState();

	LiftComponent->Drop(1, 1);
	ClimbComponent->StopClimb();
}
void ACharacterBig::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
	
	if (!bIsClimbing && !bIsHugging && !bIsSprinting && !bSuccesfulHug && !bHasDied)
	{
		if (!LiftComponent->Holding && !LiftComponent->HoldingSomething())
		{
			LiftComponent->Lift();
			if (LiftComponent->Holding)
				bIsLifting = true;
		}
		else if (LiftComponent->Holding)
		{
			LiftComponent->StartThrow();
			bIsThrowing = true;
			bIsLifting = false;
		}
	}
}

void ACharacterBig::BeginHug(const FInputActionValue& Value)
{
	if (!bIsClimbing && !bIsLifting && !bIsThrowing && !bIsSprinting && !bSuccesfulHug)
		Super::BeginHug(Value);
}

void ACharacterBig::Climb(const FInputActionValue& Value)
{
	if (LiftComponent -> HoldingSomething() == false && !bIsHugging)
	{
		ClimbComponent->Climb();
		bIsClimbing = ClimbComponent->IsClimbing();
	}
	
}

void ACharacterBig::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		if (ClimbComponent && ClimbComponent->IsClimbing())
		{
			if (!ClimbComponent->IsOnLedge() || MovementVector.Y < 0.f)
			{
				FVector ClimbDirection = FVector::UpVector;
				AddMovementInput(ClimbDirection, MovementVector.Y);
			}
			return;
		}
		//kanske en provisorisk lösning? eftersom climb inte triggas en andra gång nu 
		bIsClimbing = ClimbComponent->IsClimbing();
		Super::Move(Value);
	}
}

void ACharacterBig::OnDeath()
{
	LiftComponent->Drop(0,0);
	bIsLifting = false;
	ClimbComponent->StopClimb();
	Super::OnDeath();
}
