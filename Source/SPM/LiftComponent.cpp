// Fill out your copyright notice in the Description page of Project Settings.


#include "LiftComponent.h"

#include "CharacterSmall.h"
#include "Kismet/BlueprintTypeConversions.h"
#include "CharacterBig.h"

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
		StartThrow();
	}
	else if (!HoldingSomething() && !OwnerMovementComponent->IsFalling())
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
void ULiftComponent::Drop(float Force, float VerticalForce)
{

	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		if (ACharacterSmall* HeldPlayer = Cast<ACharacterSmall>(PhysicsHandle->GetGrabbedComponent()->GetOwner())){
			HeldPlayer->LaunchCharacter(GetOwner()->GetActorForwardVector() * Force + FVector(0,0, 1) * VerticalForce, true, true);
		}
		else
		{
			PhysicsHandle->GetGrabbedComponent()->SetPhysicsLinearVelocity(GetOwner()->GetActorForwardVector() * Force + FVector(0,0, 1) * VerticalForce);	
		}
		PhysicsHandle->GetGrabbedComponent()->GetOwner()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		PhysicsHandle->GetGrabbedComponent()->GetOwner()->SetActorEnableCollision(true);
		Release();

		GetOwner()->Tags.Remove("IsLifting");
	}

}

void ULiftComponent::StartThrow()
{
	if (bThrowing != true)
	{
		bThrowing = true;
	}
}

void ULiftComponent::GrabEffect()
{
	OriginalMovementSpeed = OwnerMovementComponent->MaxWalkSpeed;
	OwnerMovementComponent->MaxWalkSpeed = OwnerMovementComponent->MaxWalkSpeed/2;
	OwnerMovementComponent->SetJumpAllowed(false);
}

void ULiftComponent::ReleaseEffect()
{
	Super::ReleaseEffect();
	OwnerMovementComponent->SetMovementMode(MOVE_Walking);
	OwnerMovementComponent->SetJumpAllowed(true);
	OwnerMovementComponent->MaxWalkSpeed = OriginalMovementSpeed;
	Cast<ACharacterBig>(GetOwner())->bIsThrowing = false;
	Cast<ACharacterBig>(GetOwner())->bIsLifting = false;
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
		Drop(ThrowingForce, VerticalThrowingForce);
		bThrowing = false;
	}
}

void ULiftComponent::Lift()
{
	if (OwnerMovementComponent && !OwnerMovementComponent->IsFalling())
	{
		Grab();
		if (PhysicsHandle->GetGrabbedComponent() != nullptr)
		{
			GetOwner()->Tags.Add("IsLifting");
			PhysicsHandle->GetGrabbedComponent()->AttachToComponent(GetOwner()->GetParentComponent(), FAttachmentTransformRules::KeepWorldTransform);
			PhysicsHandle->GetGrabbedComponent()->GetOwner()->SetActorEnableCollision(false);
			if (ACharacterSmall* HeldPlayer = Cast<ACharacterSmall>(PhysicsHandle->GetGrabbedComponent()->GetOwner()))
			{
				HeldPlayer->ResetPlayerState();
			}
		}
	}

}
// Called every frame
void ULiftComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (Holding && PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		FVector TargetLocation1 = GetOwner()->GetComponentByClass<USkeletalMeshComponent>()->GetSocketLocation(FName("LiftHoldLocation"));
		FVector TargetLocation2 = GetOwner()->GetComponentByClass<USkeletalMeshComponent>()->GetSocketLocation(FName("LiftHoldLocation2"));
		FVector TargetLocation = (TargetLocation1 + TargetLocation2)/2;
		PhysicsHandle->GetGrabbedComponent()->GetOwner()->SetActorLocation(TargetLocation);
		PhysicsHandle->GetGrabbedComponent()->SetWorldRotation(GetOwner()->GetActorRotation());
		
		AActor* GrabbedActor = PhysicsHandle->GetGrabbedComponent()->GetOwner();
		if (!GrabbedActor->Tags.Contains("Grabbed") || Cast<ACharacterSmall>(GrabbedActor) && Cast<ACharacterSmall>(GrabbedActor)->bHasDied)
		{
			Drop(DroppingForce, VerticalDroppingForce);
		}
	}

	if (bThrowing)
	{
		ThrowTimer += DeltaTime;
		if (ThrowTimer >= ThrowTime)
		{
			ThrowTimer = 0;
			Throw();
		}
	}
	
}


