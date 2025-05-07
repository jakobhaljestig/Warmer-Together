// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabComponent.h"

#include "PhysicsEngine/PhysicsHandleComponent.h"

// Sets default values for this component's properties
UGrabComponent::UGrabComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

//Determine if player grabs or drops an object
void UGrabComponent::GrabAndRelease()
{
	if (PhysicsHandle == nullptr)
	{
		return;
	}
	
	if (PhysicsHandle->GetGrabbedComponent() != nullptr)
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
	PhysicsHandle = GetPhysicsHandle();
	OwnerMovementComponent = Cast<UCharacterMovementComponent>(GetOwner()->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No PhysicsHandle"));
	}
}


bool UGrabComponent::HoldingSomething() const
{
	for (UActorComponent* ActorComponent : GetOwner()->GetComponents())
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

	if (PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		PhysicsHandle->GetGrabbedComponent()->WakeRigidBody();
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
		if (PhysicsHandle->GetGrabbedComponent() != nullptr)
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
		HitActor->AttachToActor(GetOwner(),FAttachmentTransformRules::KeepWorldTransform);
	}
	
}

void UGrabComponent::Release()
{
	if (PhysicsHandle != nullptr && PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		Holding = false;
		AActor* GrabbedActor = PhysicsHandle->GetGrabbedComponent()->GetOwner();
		GrabbedActor->Tags.Remove("Grabbed");
		PhysicsHandle->ReleaseComponent();
		ReleaseEffect();
	}
}

//Check if an object is in reach
bool UGrabComponent::GetGrabbableInReach(FHitResult& OutHitResult) const
{
	FVector Start = GetOwner()->GetActorLocation();
	FVector End = Start + GetOwner()->GetActorForwardVector() * GrabDistance;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 2);
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
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (Result == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No PhysicsHandle found."));
	}
	return Result;
}

