// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"

#include "Engine/StaticMeshActor.h"
#include "Gameframework/CharacterMovementComponent.h"

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
	SetRelativeLocation(GetOwner()->GetActorLocation());
	SetRelativeRotation(GetOwner()->GetActorRotation());
	UPhysicsHandleComponent *PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle == nullptr)
	{
		return;
	}

	FVector TargetLocation = GetComponentLocation() + GetForwardVector() * GrabDistance;
	PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	
}
void UPickup::Grab(){
	
	
	UPhysicsHandleComponent *PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle == nullptr)
	{
		return;
	}

	FHitResult HitResult;
	bool HasHit = GetGrabbableInReach(HitResult);
	if (Holding)
	{
		Release();
	}
	else if (HasHit)
	{	Holding = true;
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		HitComponent->SetSimulatePhysics(true);
		AActor* HitActor = HitResult.GetActor();
		HitActor->Tags.Add("Grabbed");
		HitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		PhysicsHandle->GrabComponentAtLocation(
			HitComponent,
			NAME_None,
			HitResult.ImpactPoint);
	}


}

void UPickup::Release()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		Holding = false;
		AActor* GrabbedActor = PhysicsHandle->GetGrabbedComponent()->GetOwner();
		GrabbedActor->Tags.Remove("Grabbed");
		PhysicsHandle->GetGrabbedComponent()->SetPhysicsLinearVelocity(GetOwner()->GetActorForwardVector() * DroppingForce);
		PhysicsHandle->ReleaseComponent();
		
	}

}

void UPickup::Throw()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent() && Holding)
	{
		Holding = false;
		AActor* GrabbedActor = PhysicsHandle->GetGrabbedComponent()->GetOwner();
		GrabbedActor->Tags.Remove("Grabbed");
		PhysicsHandle->GetGrabbedComponent()->SetPhysicsLinearVelocity(GetOwner()->GetActorForwardVector() * ThrowingForce);
		PhysicsHandle->ReleaseComponent();
		
		
	}
}

UPhysicsHandleComponent* UPickup::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (Result == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No PhysicsHandle"));
	}
	return Result;

}
bool UPickup::GetGrabbableInReach(FHitResult& OutHitResult) const
{
	FVector Start = GetOwner()->GetActorLocation();
	FVector End = Start + GetForwardVector() * 100;
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
	UPhysicsHandleComponent *PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		FVector TargetLocation = GetOwner()->GetActorLocation() + FVector(0, 0, GrabDistance);
		PhysicsHandle->GetGrabbedComponent()->SetRelativeLocation(TargetLocation);
		PhysicsHandle->GetGrabbedComponent()->SetRelativeRotation(FRotator(GetOwner()->GetActorRotation()));
	}
}

