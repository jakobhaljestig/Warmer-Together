// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSmall.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"


void ACharacterSmall::BeginPlay()
{
	Super::BeginPlay();

	ClimbingComponent = FindComponentByClass<UClimbComponent>();
	if (!ClimbingComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Climbing component not valid"));
	}
	
	GetCharacterMovement()->JumpZVelocity = 900.0f; 
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->BrakingDecelerationFalling = 0.0f;

}


void ACharacterSmall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		
		EnhancedInputComponent->BindAction(CrawlAction, ETriggerEvent::Started, this, &ACharacterSmall::Crawl);
		EnhancedInputComponent->BindAction(CrawlAction, ETriggerEvent::Completed, this, &ACharacterSmall::StopCrawl);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ACharacterSmall::Sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ACharacterSmall::StopSprint);

		EnhancedInputComponent->BindAction(ClimbAction, ETriggerEvent::Started, this, &ACharacterSmall::Climb);
	
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
	Tags.Remove("Grabbed");
	ACharacter::Crouch(true);
}

void ACharacterSmall::StopCrawl(const FInputActionValue& Value)
{
	UE_LOG(LogTemplateCharacter, Display, TEXT("Player Small is not Crouching"));
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	ACharacter::UnCrouch(true);
}

void ACharacterSmall::Sprint(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = 800.0f;
}

void ACharacterSmall::StopSprint(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
}

void ACharacterSmall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ACharacterSmall::Climb(const FInputActionValue& Value)
{
	ClimbingComponent->Climb();
}
