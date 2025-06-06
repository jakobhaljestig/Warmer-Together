// Fill out your copyright notice in the Description page of Project Settings.


#include "PushableProperties.h"

#include "PushComponent.h"

// Sets default values for this component's properties
UPushableProperties::UPushableProperties()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UPushableProperties::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
}


// Called every frame
void UPushableProperties::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}

bool UPushableProperties::CanPush() const
{
	return NumberOfGrabbers >= RequiredNumberOfGrabbers && GrabbersHaveSameRotation();
}

FVector UPushableProperties::GetPushPosition() const
{
	FVector CombinedVectors;
	for (UPushComponent* PushComponent : Grabbers)
	{
		CombinedVectors += PushComponent->GetOwner()->GetActorLocation();
	}

	return CombinedVectors / NumberOfGrabbers;
}

bool UPushableProperties::GrabbersHaveSameRotation() const
{
	if (NumberOfGrabbers > 1)
	{
		float Yaw = 0;
		for (UPushComponent* PushComponent : Grabbers)
		{
			if (Yaw == 0)
			{
				Yaw = PushComponent->GetOwner()->GetActorRotation().Yaw;
			}
			else
			{
				Yaw -= PushComponent->GetOwner()->GetActorRotation().Yaw;
			}
		}
		if (Yaw >= 20 || Yaw <= -20)
		{
			return false;
		}
	}
	return true;
}



