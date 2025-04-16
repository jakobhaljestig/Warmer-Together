// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"

#include "CharacterSmall.h"


// Sets default values for this component's properties
UPickup::UPickup()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPickup::BeginPlay()
{
	Super::BeginPlay();
	PhysicsHandle = GetPhysicsHandle();
	
	if (PhysicsHandle == nullptr)
	{
		return;
	}

	FVector TargetLocation = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * GrabDistance;
	PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetOwner()->GetActorRotation());
	
}

//Grab Object
void UPickup::Grab(){
	
	if (PhysicsHandle == nullptr)
	{
		return;
	}

	FHitResult HitResult;
	bool HasHit = GetGrabbableInReach(HitResult);
	if (HasHit)
	{	Holding = true;
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		
		AActor* HitActor = HitResult.GetActor();
		HitComponent->SetSimulatePhysics(false);
		HitActor->Tags.Add("Grabbed");
		PhysicsHandle->GrabComponentAtLocation(
			HitComponent,
			NAME_None,
			HitResult.ImpactPoint);
		HitActor->AttachToComponent(this->GetOwner()->GetParentComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	}
}
//Drop object
void UPickup::Drop(float Force)
{

	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		Holding = false;
		AActor* GrabbedActor = PhysicsHandle->GetGrabbedComponent()->GetOwner();
		GrabbedActor->Tags.Remove("Grabbed");
		PhysicsHandle->GetGrabbedComponent()->SetPhysicsLinearVelocity(GetOwner()->GetActorForwardVector() * Force + FVector(0,0, 1) * DroppingForce);
		PhysicsHandle->GetGrabbedComponent()->SetSimulatePhysics(true);
		PhysicsHandle->GetGrabbedComponent()->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
		PhysicsHandle->ReleaseComponent();
	}

}
//Call drop with more force
void UPickup::Throw()
{
	if (PhysicsHandle == nullptr)
	{
		return;
	}
	
	Drop(ThrowingForce);
}
//Get Physics Handle
UPhysicsHandleComponent* UPickup::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (Result == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No PhysicsHandle"));
	}
	return Result;

}
//Check if an object is in reach
bool UPickup::GetGrabbableInReach(FHitResult& OutHitResult) const
{
	FVector Start = GetOwner()->GetActorLocation();
	FVector End = Start + GetOwner()->GetActorForwardVector() * GrabDistance;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	return GetWorld()->SweepSingleByChannel(
		OutHitResult,
		Start, End,
		FQuat::Identity,
		ECC_GameTraceChannel1,
		Sphere);
}

// Called every frame
void UPickup::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		FVector TargetLocation = GetOwner()->GetActorLocation() + FVector(0,0,GrabDistance);
		PhysicsHandle->GetGrabbedComponent()->SetRelativeLocation(TargetLocation);
		PhysicsHandle->GetGrabbedComponent()->SetWorldRotation(this->GetOwner()->GetActorRotation());
	}
}
//Determine if player grabs or drops an object
void UPickup::PickUpOrDrop()
{
	if (PhysicsHandle == nullptr)
	{
		return;
	}
	
	if (Holding)
	{
		Drop(DroppingForce);
	}
	else
	{
		Grab();
	}
	
}

