// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabComponent.h"

#include "CharacterBig.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

// Sets default values for this component's properties
UGrabComponent::UGrabComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

//Determine if player grabs or drops an object
void UGrabComponent::GrabAndRelease()
{
	if (PhysicsHandle == nullptr)
	{
		return;
	}
	
	if (GrabbedComponent != nullptr)
	{
		Release();
	}
	else if (!HoldingSomething())
	{
		Grab();
	}
	
}


// Called when the game starts
void UGrabComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	OwnerMovementComponent = Owner->GetComponentByClass<UCharacterMovementComponent>();
	PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No PhysicsHandle"));
	}
}

bool UGrabComponent::HoldingSomething() const
{
	for (UActorComponent* ActorComponent : Owner->GetComponents())
	{
		if (Cast<UGrabComponent>(ActorComponent) != nullptr)
		{
			if (Cast<UGrabComponent>(ActorComponent)->Holding)
			{
				return true;
			}
		}
	}
	return false;
}

// Called every frame
void UGrabComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GrabbedComponent != nullptr)
	{
		GrabbedComponent->WakeRigidBody();
	}
	// ...
}
//Grab Object
void UGrabComponent::Grab(){
	
	if (PhysicsHandle == nullptr)
	{
		return;
	}

	FHitResult HitResult;
	if (GetGrabbableInReach(HitResult))
	{
		if (GrabbedComponent != nullptr)
		{
			return;
		}

		
		
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		Holding = true;
		AActor* HitActor = HitResult.GetActor();
		HitComponent->WakeAllRigidBodies();
		HitActor->Tags.Add("Grabbed");
		HitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		PhysicsHandle->GrabComponentAtLocation(
			HitComponent,
			NAME_None,
			HitResult.ImpactPoint);
		GrabEffect();
		GrabbedComponent = HitComponent;
		GrabbedActor = HitActor;
	}
	
}

void UGrabComponent::Release()
{
	if (PhysicsHandle != nullptr && GrabbedComponent != nullptr)
	{
		Holding = false;
		GrabbedActor->Tags.Remove("Grabbed");
		
		PhysicsHandle->ReleaseComponent();
		ReleaseEffect();
		GrabbedComponent = nullptr;
		GrabbedActor = nullptr;
	}
}

//Check if an object is in reach
bool UGrabComponent::GetGrabbableInReach(FHitResult& OutHitResult) const
{
	FVector Start = Owner->GetActorLocation();
	FVector End = Start + Owner->GetActorForwardVector() * GrabDistance;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	return GetWorld()->SweepSingleByChannel(
		OutHitResult,
		Start, End,
		FQuat::Identity,
		CollisionChannel,
		Sphere);
	
}
bool UGrabComponent::GetGrabbableInReach(FHitResult& OutHitResult, float Distance) const
{
	FVector Start = Owner->GetActorLocation();
	FVector End = Start + Owner->GetActorForwardVector() * Distance;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	return GetWorld()->SweepSingleByChannel(
		OutHitResult,
		Start, End,
		FQuat::Identity,
		CollisionChannel,
		Sphere);
	
}

void UGrabComponent::GrabEffect()
{
	
}

void UGrabComponent::ReleaseEffect()
{
}

UPhysicsHandleComponent* UGrabComponent::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* Result = Cast<ACharacterBase>(Owner)->GetPhysicsHandle();
	if (Result == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No PhysicsHandle found."));
	}
	return Result;
}

