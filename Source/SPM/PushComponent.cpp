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
			PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetOwner()->GetActorRotation());
		}
		
		FHitResult Hit;
		if (!GetGrabbableInReach(Hit) || OwnerMovementComponent->IsFalling())
		{
			StopPushing();
		}
	}
}
//Exists in case something should be added to the execution of push.
void UPushComponent::StartPushing()
{
	if (!HoldingSomething() && !OwnerMovementComponent->IsFalling())
		Grab();
	
}
//Restores player movement and drops grabbed object
void UPushComponent::StopPushing()
{
	if (PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		PhysicsHandle->GetGrabbedComponent()->SetPhysicsLinearVelocity(FVector(0, 0, 0));
		Release();	
	}
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

void UPushComponent::ReleaseEffect()
{
	Super::ReleaseEffect();
	OwnerMovementComponent->MaxWalkSpeed = OriginalMovementSpeed;
	OwnerMovementComponent->RotationRate = OriginalRotationRate;
	OwnerMovementComponent->SetJumpAllowed(true);
}

