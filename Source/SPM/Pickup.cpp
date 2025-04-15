// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"

#include "MovieSceneTracksComponentTypes.h"


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
	UPhysicsHandleComponent *PhysicsHandle = GetPhysicsHandle();
	
	if (PhysicsHandle == nullptr)
	{
		return;
	}

	FVector TargetLocation = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * GrabDistance;
	PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetOwner()->GetActorRotation());
	
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
		HitComponent->SetSimulatePhysics(false);
		AActor* HitActor = HitResult.GetActor();
		HitActor->Tags.Add("Grabbed");
		PhysicsHandle->GrabComponentAtLocation(
			HitComponent,
			NAME_None,
			HitResult.ImpactPoint);
		HitActor->AttachToComponent(this->GetOwner()->GetParentComponent(), FAttachmentTransformRules::KeepRelativeTransform);
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
		PhysicsHandle->GetGrabbedComponent()->SetPhysicsLinearVelocity(GetOwner()->GetActorForwardVector() * DroppingForce + FVector(0,0, 1) * DroppingForce);
		PhysicsHandle->GetGrabbedComponent()->SetSimulatePhysics(true);
		PhysicsHandle->GetGrabbedComponent()->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
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
		PhysicsHandle->GetGrabbedComponent()->SetPhysicsLinearVelocity(GetOwner()->GetActorForwardVector() * ThrowingForce + FVector(0,0, 1) * DroppingForce);
		PhysicsHandle->GetGrabbedComponent()->SetSimulatePhysics(true);
		PhysicsHandle->GetGrabbedComponent()->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
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
	UPhysicsHandleComponent *PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		FVector TargetLocation = GetOwner()->GetActorLocation() + FVector(0,0,GrabDistance);
		PhysicsHandle->GetGrabbedComponent()->SetRelativeLocation(TargetLocation);
		PhysicsHandle->GetGrabbedComponent()->SetWorldRotation(this->GetOwner()->GetActorRotation());
	}
}

