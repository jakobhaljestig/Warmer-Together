// Fill out your copyright notice in the Description page of Project Settings.


#include "FreezingTouchComponent.h"

#include "CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values for this component's properties
UFreezingTouchComponent::UFreezingTouchComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}

// Called when the game starts
void UFreezingTouchComponent::BeginPlay()
{
	Super::BeginPlay();
	TriggerBox = GetOwner()->GetComponentByClass<UBoxComponent>();
}


// Called every frame
void UFreezingTouchComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (TriggerBox != nullptr)
	{
		TArray<UPrimitiveComponent*> OverlappingActors;
		TriggerBox->GetOverlappingComponents(OverlappingActors);
		for (UPrimitiveComponent* Actor : OverlappingActors)
		{
			if (ACharacterBase* Character = Cast<ACharacterBase>(Actor->GetOwner()))
			{
				if (Character->GetComponentByClass<UBodyTemperature>()->GetTempPercentage() > 0)
					Character->GetComponentByClass<UBodyTemperature>()->ModifyTemperature(-CoolDownRate*DeltaTime);
				/*else
				{
					Character->GetComponentByClass<UHealth>()->TakeDamage(CoolDownRate * DeltaTime);
				}*/
			}
		}
	}
	// ...
}

