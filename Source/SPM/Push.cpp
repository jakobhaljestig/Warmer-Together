// Fill out your copyright notice in the Description page of Project Settings.


#include "PushPull.h"

UPush::UPush()
{
	PrimaryComponentTick.bCanEverTick = true;
	TraceChannel = ECC_GameTraceChannel2;
}

void UPush::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (Holding && PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		FVector TargetLocation = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * GrabDistance*2;
		PhysicsHandle->GetGrabbedComponent()->SetRelativeLocation(TargetLocation);
		PhysicsHandle->GetGrabbedComponent()->SetWorldRotation(this->GetOwner()->GetActorRotation());
	}
}

void UPush::GrabAndRelease()
{
	if (PhysicsHandle == nullptr)
	{
		return;
	}
	
	if (Holding && !Lifting)
	{
		StopPushing();
	}
	else if (!Lifting)
	{
		StartPushing();
	}
}

void UPush::StartPushing()
{
	if (PhysicsHandle == nullptr)
	{
		return;
	}

	FHitResult HitResult;
	bool HasHit = GetGrabbableInReach(HitResult);
	if (HasHit)
	{	
		Holding = true;
		PhysicsGrab(HitResult);
	}
}
void UPush::StopPushing()
{
	if (Holding)
	{
		Holding = false;
		AActor* GrabbedActor = PhysicsHandle->GetGrabbedComponent()->GetOwner();
		GrabbedActor->Tags.Remove("Grabbed");
		PhysicsHandle->GetGrabbedComponent()->SetSimulatePhysics(true);
		PhysicsHandle->GetGrabbedComponent()->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
		PhysicsHandle->ReleaseComponent();
	}
	
}

