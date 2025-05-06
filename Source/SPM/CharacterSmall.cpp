// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSmall.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"


void ACharacterSmall::BeginPlay()
{
	Super::BeginPlay();
	
	GetCharacterMovement()->JumpZVelocity = 800.0f; 
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->BrakingDecelerationFalling = -1000.0f;

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	
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
		
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

//Implementera check för att se om något är över karaktären.
void ACharacterSmall::Crawl (const FInputActionValue& Value)
{
	if (!bIsSprinting && !bIsPushing)
	{
		UE_LOG(LogTemplateCharacter, Display, TEXT("Player Small is Crouching"));
		GetCharacterMovement()->MaxWalkSpeed = 250.0f;
		Tags.Remove("Grabbed");
		ACharacter::Crouch(true);
		bIsCrawling = true;
	}
}

void ACharacterSmall::StopCrawl(const FInputActionValue& Value)
{
	if (bIsCrawling)
	{
		UE_LOG(LogTemplateCharacter, Display, TEXT("Player Small is not Crouching"));
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;
		ACharacter::UnCrouch(true);
		bIsCrawling = false;
	}
}

void ACharacterSmall::Sprint(const FInputActionValue& Value)
{
	if (!bIsPushing && !bIsCrawling)
	{
		GetCharacterMovement()->MaxWalkSpeed = 800.0f;
		bIsSprinting = true;
	}
}

void ACharacterSmall::StopSprint(const FInputActionValue& Value)
{
	if (bIsSprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;
		bIsSprinting = false;
	}
}

void ACharacterSmall::BeginPush(const FInputActionValue& Value)
{
	if (!bIsSprinting && !bIsCrawling)
		Super::BeginPush(Value);
}

void ACharacterSmall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}





