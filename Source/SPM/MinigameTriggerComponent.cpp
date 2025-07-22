// Fill out your copyright notice in the Description page of Project Settings.


#include "MinigameTriggerComponent.h"

#include "BodyTemperature.h"
#include "CharacterBase.h"
#include "CharacterBig.h"
#include "CharacterSmall.h"
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

	MiniGamePawn = Cast<APawn>(GetOwner());
	
	
	// ...
	
}

void UMinigameTriggerComponent::ZoomIn(AActor* Actor)
{
	Controller = Cast<ACharacterPlayerController>(ControllerOwner->GetController());
	if (ControllerOwner && Controller)
	{
		Cast<ACharacterBase>(ControllerOwner)->GetBodyTemperature()->SetCoolDownRate(0);
		Controller->SetCanShiver(false);
		bActive = true;
		ControllerOwner->GetMovementComponent()->Velocity = FVector(0, 0, 0);
		Controller->HideHUD(true);
		Controller->Possess(MiniGamePawn);
		Controller->SetViewTarget(ControllerOwner);
		Controller->SetViewTargetWithBlend(MiniGamePawn, 1, VTBlend_EaseIn, 5, true);
	}
}

void UMinigameTriggerComponent::ZoomOut()
{
	bActive = false;
	bCompleted = true;
	if (ControllerOwner && Controller)
	{
		Controller->Possess(ControllerOwner);
		Controller->SetViewTarget(MiniGamePawn);
		Controller->SetViewTargetWithBlend(ControllerOwner, 1, VTBlend_EaseIn, 5, true);
		ControllerOwner = nullptr;
	}
}

void UMinigameTriggerComponent::Start(ACharacterBase* Character)
{
	if (!bCompleted && !bActive)
	{
		if (ForBigPlayer)
		{
			if (!Cast<ACharacterBig>(Character) && !Controller)
			{
				return;
			}
		}
		if (ForSmallPlayer)
		{
			if (!Cast<ACharacterSmall>(Character) && !Controller)
			{
				return;
			}
		}
	}
	
	ControllerOwner = Cast<ACharacter>(Character);
	if (ControllerOwner && !bActive){
		if (Cast<ACharacterBig>(ControllerOwner) && Cast<ACharacterBig>(ControllerOwner)->bIsClimbing)
		{
			return;
		}
		ZoomIn(ControllerOwner);
	}
}


// Called every frame
void UMinigameTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}




