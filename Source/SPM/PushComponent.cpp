// Fill out your copyright notice in the Description page of Project Settings.


#include "PushComponent.h"



UPushComponent::UPushComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CollisionChannel = ECC_GameTraceChannel2;
}

void UPushComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (Holding && PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		if (PhysicsHandle->GetGrabbedComponent()->GetMass() < MaxPushWeight){
			FVector TargetLocation = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * HoldDistance;
			PhysicsHandle->SetTargetLocation(TargetLocation);
		}
		if ((PhysicsHandle->GetGrabbedComponent()->GetComponentLocation() - GetOwner()->GetActorLocation()).Length() > HoldDistance * 4)
		{
			StopPushing();
		}
	}
}

void UPushComponent::GrabAndRelease()
{
	if (PhysicsHandle == nullptr)
	{
		return;
	}
	
	if (Holding && PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		StopPushing();
	}
	else if (!HoldingSomething() && !OwnerMovementComponent->IsFalling())
	{
		StartPushing();
	}
}
//Exists in case something should be added to the execution of push.
void UPushComponent::StartPushing()
{
	Grab();
	
}
//Restores player movement and drops grabbed object
void UPushComponent::StopPushing()
{
	PhysicsHandle->GetGrabbedComponent()->SetPhysicsLinearVelocity(FVector(0, 0, 0));
	Release();
	OwnerMovementComponent->MaxWalkSpeed = OriginalMovementSpeed;
	OwnerMovementComponent->RotationRate = OriginalRotationRate;
	OwnerMovementComponent->SetJumpAllowed(true);
}
//Restricts player movement
void UPushComponent::GrabEffect()
{
	OriginalMovementSpeed = OwnerMovementComponent->MaxWalkSpeed;
	OriginalRotationRate = OwnerMovementComponent->RotationRate;
	OwnerMovementComponent->MaxWalkSpeed = OwnerMovementComponent->MaxWalkSpeed/4;
	OwnerMovementComponent->RotationRate = FRotator(0, 0, 0);
	OwnerMovementComponent->SetJumpAllowed(false);
}

