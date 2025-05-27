// Fill out your copyright notice in the Description page of Project Settings.


#include "MinigameTriggerComponent.h"

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
	TriggerBox = GetOwner()->GetComponentByClass<UBoxComponent>();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &UMinigameTriggerComponent::OnBeginOverlap);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &UMinigameTriggerComponent::OnEndOverlap);
	MiniGamePawn = Cast<APawn>(GetOwner());
	
	
	// ...
	
}

void UMinigameTriggerComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (TriggerBox && !bCompleted && !bActive)
	{
		if (ForBigPlayer)
		{
			if (ACharacterBase* Character = Cast<ACharacterBig>(OtherActor))
			{
				ControllerOwner = Character;
			}
		}
		if (ForSmallPlayer)
		{
			if (ACharacterBase* Character = Cast<ACharacterSmall>(OtherActor))
			{
				ControllerOwner = Character;
			}
		}
	}
	
}

void UMinigameTriggerComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!bCompleted && !bActive)
	{
		if (ForBigPlayer)
		{
			if (Cast<ACharacterBig>(OtherActor) && !Controller)
			{
				ControllerOwner = nullptr;
			}
		}
		if (ForSmallPlayer)
		{
			if (Cast<ACharacterSmall>(OtherActor) && !Controller)
			{
				ControllerOwner = nullptr;
			}
		}
	}
}

void UMinigameTriggerComponent::ZoomIn(AActor* Actor)
{
	Controller = Cast<APlayerController>(ControllerOwner->GetController());
	if (ControllerOwner && Controller)
	{
		TriggerBox->RemoveFromRoot();
		bActive = true;
		ControllerOwner->GetMovementComponent()->Velocity = FVector(0, 0, 0);
		Controller->Possess(MiniGamePawn);
		Controller->SetViewTarget(ControllerOwner);
		Controller->SetViewTargetWithBlend(MiniGamePawn, 1, VTBlend_EaseIn, 5, true);
	}
}

void UMinigameTriggerComponent::ZoomOut()
{
	bActive = false;
	bCompleted = true;
	if (ControllerOwner && Cast<APawn>(GetOwner())->GetController())
	{
		Controller->Possess(ControllerOwner);
		Cast<ACharacterBase>(ControllerOwner)->bIsHugging = false;
		ControllerOwner = nullptr;
	}
	
}


// Called every frame
void UMinigameTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...

	if (ControllerOwner && !bActive && Cast<ACharacterBase>(ControllerOwner)->bIsHugging){
		ZoomIn(ControllerOwner);
	}
}




