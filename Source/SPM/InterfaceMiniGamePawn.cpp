// Fill out your copyright notice in the Description page of Project Settings.


#include "InterfaceMiniGamePawn.h"

#include "EnhancedInputComponent.h"

// Sets default values
AInterfaceMiniGamePawn::AInterfaceMiniGamePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInterfaceMiniGamePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInterfaceMiniGamePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AInterfaceMiniGamePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)){
		EnhancedInputComponent->BindAction(Up, ETriggerEvent::Triggered, this, &AInterfaceMiniGamePawn::UpPressed);

		EnhancedInputComponent->BindAction(Down, ETriggerEvent::Triggered, this, &AInterfaceMiniGamePawn::DownPressed);

		EnhancedInputComponent->BindAction(Right, ETriggerEvent::Triggered, this, &AInterfaceMiniGamePawn::RightPressed);

		EnhancedInputComponent->BindAction(Left, ETriggerEvent::Triggered, this, &AInterfaceMiniGamePawn::LeftPressed);
	}

}

void AInterfaceMiniGamePawn::UpPressed(const FInputActionValue& Value)
{
	
}

void AInterfaceMiniGamePawn::DownPressed(const FInputActionValue& Value)
{
	
}

void AInterfaceMiniGamePawn::RightPressed(const FInputActionValue& Value)
{
	
}

void AInterfaceMiniGamePawn::LeftPressed(const FInputActionValue& Value)
{
	
}

