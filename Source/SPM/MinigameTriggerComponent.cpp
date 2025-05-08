// Fill out your copyright notice in the Description page of Project Settings.


#include "MinigameTriggerComponent.h"

#include "CharacterBase.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values for this component's properties
UMinigameTriggerComponent::UMinigameTriggerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMinigameTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
	TriggerBox = GetOwner()->GetComponentByClass<UBoxComponent>();
	MiniGamePawn = Cast<APawn>(GetOwner());
	// ...
	
}

void UMinigameTriggerComponent::StartMiniGame()
{
	
}


void UMinigameTriggerComponent::ZoomIn(UPrimitiveComponent* Actor)
{
	ControllerOwner = Cast<ACharacterBase>(Actor->GetOwner());
	Controller = Cast<APlayerController>(ControllerOwner->GetController());
	if (ControllerOwner && Controller)
	{
		bActive = true;
		ControllerOwner->GetComponentByClass<UBodyTemperature>()->SetCoolDownRate(0);
		Controller->Possess(MiniGamePawn);
		Controller->SetViewTarget(ControllerOwner);
		Controller->SetViewTargetWithBlend(MiniGamePawn, 1, VTBlend_EaseIn, 5, true);
	}
}

void UMinigameTriggerComponent::ZoomOut(UPrimitiveComponent* Actor)
{
	bActive = false;
	ControllerOwner = Cast<ACharacterBase>(Actor->GetOwner());
	Controller = Cast<APlayerController>(ControllerOwner->GetController());
	Controller->SetViewTargetWithBlend(ControllerOwner, 1, VTBlend_EaseOut, 5, true);
	ControllerOwner->GetComponentByClass<UBodyTemperature>()->SetCoolDownRate(0.75);
}


// Called every frame
void UMinigameTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (TriggerBox && !bCompleted && !bActive)
	{
		TArray<UPrimitiveComponent*> OverlappingActors;
		TriggerBox->GetOverlappingComponents(OverlappingActors);
		for (UPrimitiveComponent* Actor : OverlappingActors)
		{
			if (ACharacterBase* Character = Cast<ACharacterBase>(Actor->GetOwner()))
			{
				if (!Character->GetCharacterMovement()->IsFalling())
					ZoomIn(Actor);
			}
		}
	}
	else if (bCompleted)
	{
		Controller->Possess(ControllerOwner);
	}

	
	// ...
}



