// Fill out your copyright notice in the Description page of Project Settings.


#include "LiftComponent.h"

#include "BodyTemperature.h"
#include "CharacterSmall.h"
#include "CharacterBig.h"
#include "Engine/SkeletalMeshSocket.h"

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
	
	if (Holding && GrabbedComponent!= nullptr)
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
	Skeleton = Owner->GetComponentByClass<USkeletalMeshComponent>();
	Hand1 = Skeleton->GetSocketByName(FName("LiftHoldLocation"));
	Hand2 = Skeleton->GetSocketByName(FName("LiftHoldLocation2"));
}
void ULiftComponent::Lift()
{
	if (OwnerMovementComponent && !OwnerMovementComponent->IsFalling())
	{
		Grab();
		if (GrabbedComponent != nullptr)
		{
			Owner->Tags.Add("IsLifting");
			GrabbedComponent->AttachToComponent(Owner->GetParentComponent(), FAttachmentTransformRules::KeepWorldTransform);
			GrabbedActor->SetActorEnableCollision(false);
			if (ACharacterSmall* HeldPlayer = Cast<ACharacterSmall>(GrabbedActor))
			{
				HeldPlayer->ResetPlayerState();
			}
		}
	}
}
void ULiftComponent::Drop(float Force, float VerticalForce)
{

	if (PhysicsHandle && GrabbedComponent)
	{
		if (ACharacterSmall* HeldPlayer = Cast<ACharacterSmall>(GrabbedActor)){
			HeldPlayer->LaunchCharacter(Owner->GetActorForwardVector() * Force + FVector(0,0, 1) * VerticalForce, true, true);
		}
		else
		{
			GrabbedComponent->SetPhysicsLinearVelocity(Owner->GetActorForwardVector() * Force + FVector(0,0, 1) * VerticalForce);	
		}
		GrabbedActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		GrabbedActor->SetActorEnableCollision(true);
		Release();
		
		Owner->Tags.Remove("IsLifting");
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
	Cast<ACharacterBig>(Owner)->bIsThrowing = false;
	Cast<ACharacterBig>(Owner)->bIsLifting = false;
}

//Call drop with more force
void ULiftComponent::Throw()
{
	if (PhysicsHandle == nullptr)
	{
		return;
	}
	if (Holding && GrabbedComponent != nullptr)
	{
		Drop(ThrowingForce, VerticalThrowingForce);
		bThrowing = false;
	}
}
// Called every frame
void ULiftComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (Holding && PhysicsHandle && GrabbedComponent)
	{
		FVector TargetLocation = (Hand1->GetSocketLocation(Skeleton) + Hand2->GetSocketLocation(Skeleton))/2;
		GrabbedActor->SetActorLocation(TargetLocation);
		GrabbedComponent->SetWorldRotation(Owner->GetActorRotation());
		
		if (!GrabbedActor->Tags.Contains("Grabbed") || Cast<ACharacterSmall>(GrabbedActor) && Cast<ACharacterSmall>(GrabbedActor)->bHasDied)
		{
			Drop(DroppingForce, VerticalDroppingForce);
		}
		else
		{
			Cast<ACharacterBase>(GrabbedActor)->GetBodyTemperature()->IsNearHeat(Cast<ACharacterBase>(Owner)->GetBodyTemperature()->bNearHeat);
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


