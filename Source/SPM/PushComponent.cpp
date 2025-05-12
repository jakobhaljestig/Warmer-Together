// Fill out your copyright notice in the Description page of Project Settings.


#include "PushComponent.h"

#include "PushableProperties.h"

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
		FHitResult Hit;
		if (!GetGrabbableInReach(Hit))
		{
			StopPushing();
		}
		else if(!PhysicsHandle->GetGrabbedComponent()->GetOwner()->GetComponentByClass<UPushableProperties>()->CanPush()){
			OwnerMovementComponent->MaxWalkSpeed = 0;
		}
		else if (PhysicsHandle->GetGrabbedComponent()->GetOwner()->GetComponentByClass<UPushableProperties>()->CanPush())
		{
			OwnerMovementComponent->MaxWalkSpeed = OriginalMovementSpeed/4;
			FVector TargetLocation = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * PhysicsHandle->GetGrabbedComponent()->GetOwner()->GetComponentByClass<UPushableProperties>()->HoldDistance;
			PhysicsHandle->SetTargetLocation(TargetLocation);
		}
		
	}
}
void UPushComponent::StartPushing()
{
	if (!HoldingSomething())
	{
		Grab();
		if (PhysicsHandle->GetGrabbedComponent() && PhysicsHandle->GetGrabbedComponent()->GetOwner()->GetComponentByClass<UPushableProperties>())
		{
			PhysicsHandle->GetGrabbedComponent()->GetOwner()->GetComponentByClass<UPushableProperties>()->NumberOfGrabbers += 1;
		}
		
	}

	
}
void UPushComponent::StopPushing()
{
	if (PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		PhysicsHandle->GetGrabbedComponent()->SetPhysicsLinearVelocity(FVector(0, 0, 0));
		if (PhysicsHandle->GetGrabbedComponent()->GetOwner()->GetComponentByClass<UPushableProperties>())
		{
			PhysicsHandle->GetGrabbedComponent()->GetOwner()->GetComponentByClass<UPushableProperties>()->NumberOfGrabbers -= 1;
		}
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

