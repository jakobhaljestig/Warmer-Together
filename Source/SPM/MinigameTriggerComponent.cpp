// Fill out your copyright notice in the Description page of Project Settings.


#include "MinigameTriggerComponent.h"

#include "CharacterBase.h"
#include "Components/BoxComponent.h"
#include "GameFramework/PawnMovementComponent.h"

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
	// ...
	
}

void UMinigameTriggerComponent::StartMiniGame()
{
	
}


void UMinigameTriggerComponent::ZoomIn(UPrimitiveComponent* Actor)
{
	ACharacterBase* Character = Cast<ACharacterBase>(Actor->GetOwner());
	APlayerController* Controller = Cast<APlayerController>(Character->GetController());
	Character->GetMovementComponent()->Deactivate();
	Controller->SetViewTargetWithBlend(GetOwner(), 1, VTBlend_EaseIn, 5, true);
	Character->GetComponentByClass<UBodyTemperature>()->SetCoolDownRate(0);
}

void UMinigameTriggerComponent::ZoomOut(UPrimitiveComponent* Actor)
{
	ACharacterBase* Character = Cast<ACharacterBase>(Actor->GetOwner());
	APlayerController* Controller = Cast<APlayerController>(Character->GetController());
	Character->GetMovementComponent()->Activate();
	Controller->SetViewTargetWithBlend(Character, 1, VTBlend_EaseOut, 5, true);
	Character->GetComponentByClass<UBodyTemperature>()->SetCoolDownRate(0.75);
}


// Called every frame
void UMinigameTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TArray<UPrimitiveComponent*> OverlappingActors;
	if (TriggerBox && !bCompleted)
	{
		TriggerBox->GetOverlappingComponents(OverlappingActors);
		for (UPrimitiveComponent* Actor : OverlappingActors)
		{
			if (Cast<ACharacterBase>(Actor->GetOwner()))
			{
				ZoomIn(Actor);
			}
		}
	}
	else
	{
		
	}

	
	// ...
}


