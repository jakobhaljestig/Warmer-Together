// Fill out your copyright notice in the Description page of Project Settings.


#include "PushComponent.h"

#include "PushableProperties.h"
#include "VectorTypes.h"

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
		if (PhysicsHandle->GetGrabbedComponent()->GetOwner()->GetComponentByClass<UPushableProperties>()->bIsFalling)
		{
			StopPushing();
		}
		else if(!PhysicsHandle->GetGrabbedComponent()->GetOwner()->GetComponentByClass<UPushableProperties>()->CanPush()){
			OwnerMovementComponent->MaxWalkSpeed = 0;
		}
		else if (PhysicsHandle->GetGrabbedComponent()->GetOwner()->GetComponentByClass<UPushableProperties>()->CanPush())
		{
			if (!GetGrabbableInReach(Hit, PhysicsHandle->GetGrabbedComponent()->GetOwner()->GetComponentByClass<UPushableProperties>()->HoldDistance*1.1))
			{
				OwnerMovementComponent->MaxWalkSpeed = 0;
				GetOwner()->SetActorLocation(GetOwner()->GetActorLocation() + OwnerMovementComponent->GetForwardVector());
			}
			else
			{
				OwnerMovementComponent->MaxWalkSpeed = OriginalMovementSpeed/4;
			}
			
			FVector TargetLocation = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * PhysicsHandle->GetGrabbedComponent()->GetOwner()->GetComponentByClass<UPushableProperties>()->HoldDistance;
			PhysicsHandle->SetTargetLocation(TargetLocation);
			
		}
		
		
	}
}
void UPushComponent::StartPushing()
{
	if (!HoldingSomething() && !Holding)
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
	if (PhysicsHandle->GetGrabbedComponent() != nullptr && Holding)
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
	OwnerMovementComponent->MaxWalkSpeed = OriginalMovementSpeed/4;
	OwnerMovementComponent->MinAnalogWalkSpeed = 0;
	OwnerMovementComponent->RotationRate = FRotator(0, 0, 0);
	OwnerMovementComponent->SetJumpAllowed(false);
	OwnerMovementComponent->SetPlaneConstraintEnabled(true);
	OwnerMovementComponent->SetPlaneConstraintNormal(GetOwner()->GetActorRightVector() * 200);
	
}

void UPushComponent::ReleaseEffect()
{
	Super::ReleaseEffect();
	OwnerMovementComponent->MinAnalogWalkSpeed = 20.f;
	OwnerMovementComponent->MaxWalkSpeed = OriginalMovementSpeed;
	OwnerMovementComponent->RotationRate = OriginalRotationRate;
	OwnerMovementComponent->SetJumpAllowed(true);
	OwnerMovementComponent->SetPlaneConstraintEnabled(false);
}

