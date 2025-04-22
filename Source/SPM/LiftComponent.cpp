// Fill out your copyright notice in the Description page of Project Settings.


#include "LiftComponent.h"


ULiftComponent::ULiftComponent()
{

	PrimaryComponentTick.bCanEverTick = true;
	CollisionChannel = ECC_GameTraceChannel1;
}

//Override standard behavior
void ULiftComponent::GrabAndRelease()
{
	if (PhysicsHandle == nullptr)
	{
		return;
	}
	
	if (Holding && PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		Drop(DroppingForce);
	}
	else if (!HoldingSomething())
	{
		Lift();
	}
}


// Called when the game starts
void ULiftComponent::BeginPlay()
{
	Super::BeginPlay();
	
}
//Drop object
void ULiftComponent::Drop(float Force)
{

	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		PhysicsHandle->GetGrabbedComponent()->SetPhysicsLinearVelocity(GetOwner()->GetActorForwardVector() * Force + FVector(0,0, 1) * DroppingForce);
		OwnerMovementComponent->SetMovementMode(MOVE_Walking);
		OwnerMovementComponent->SetJumpAllowed(true);
		OwnerMovementComponent->MaxWalkSpeed = OriginalMovementSpeed;
		Release();
		
	}

}

void ULiftComponent::GrabEffect()
{
	OriginalMovementSpeed = OwnerMovementComponent->MaxWalkSpeed;
	OwnerMovementComponent->MaxWalkSpeed = 0;
	OwnerMovementComponent->SetJumpAllowed(false);
}

//Call drop with more force
void ULiftComponent::Throw()
{
	if (PhysicsHandle == nullptr)
	{
		return;
	}
	if (Holding && PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		Drop(ThrowingForce);
	}
}

void ULiftComponent::Lift()
{
	Grab();

}
// Called every frame
void ULiftComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (Holding && PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		FVector TargetLocation = GetOwner()->GetActorLocation() + FVector(0,0,HoldDistance);
		PhysicsHandle->GetGrabbedComponent()->SetRelativeLocation(TargetLocation);
		PhysicsHandle->GetGrabbedComponent()->SetWorldRotation(this->GetOwner()->GetActorRotation());
	}
}


