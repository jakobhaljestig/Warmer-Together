// Fill out your copyright notice in the Description page of Project Settings.


#include "PushComponent.h"
#include "PushableProperties.h"

UPushComponent::UPushComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CollisionChannel = ECC_GameTraceChannel2;
}

void UPushComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (Holding && PhysicsHandle && GrabbedComponent)
	{
		FHitResult Hit;

		if (GrabbedComponent->GetOwner()->GetComponentByClass<UPushableProperties>()->bIsFalling || OwnerMovementComponent->IsFalling())
		{
			StopPushing();
		}
		else if(!GrabbedComponent->GetOwner()->GetComponentByClass<UPushableProperties>()->CanPush()){
			UpdateGrabLocation();
			OwnerMovementComponent->MaxWalkSpeed = 0;
		}
		else if (GrabbedComponent->GetOwner()->GetComponentByClass<UPushableProperties>()->CanPush())
		{
			OwnerMovementComponent->MaxWalkSpeed = OriginalMovementSpeed/4;
			if (!GetGrabbableInReach(Hit, PhysicsHandle->GetGrabbedComponent()->GetOwner()->GetComponentByClass<UPushableProperties>()->HoldDistance*1.1))
			{
				//If player is too far away, try to move back into reach, otherwise stop pushing
				int Attempts = 0;
				while (!GetGrabbableInReach(Hit, GrabbedComponent->GetOwner()->GetComponentByClass<UPushableProperties>()->HoldDistance*1.1) && OwnerMovementComponent->Velocity.Length() > PhysicsHandle->GetGrabbedComponent()->GetComponentVelocity().Length())
				{
					Attempts++;
					GetOwner()->SetActorLocation(GetOwner()->GetActorLocation() + OwnerMovementComponent->GetForwardVector());
					if (Attempts >= 5) break;
				}
				if (!GetGrabbableInReach(Hit))
				{
					StopPushing();
				}
				
			}
			if (GrabbedComponent != nullptr && GrabbedComponent->GetOwner()->GetComponentByClass<UPushableProperties>()->CanPush())
			{
				FVector TargetLocation =  GrabbedComponent->GetOwner()->GetComponentByClass<UPushableProperties>()->GetPushPosition() + GetOwner()->GetActorForwardVector() * GrabbedComponent->GetOwner()->GetComponentByClass<UPushableProperties>()->HoldDistance;
				PhysicsHandle->SetTargetLocation(TargetLocation);
			}
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
			GrabbedComponent->GetOwner()->GetComponentByClass<UPushableProperties>()->NumberOfGrabbers += 1;
			GrabbedComponent->GetOwner()->GetComponentByClass<UPushableProperties>()->Grabbers.Add(this); 
		}
		
	}

	
}
void UPushComponent::StopPushing()
{
	if (GrabbedComponent != nullptr && Holding)
	{
		GrabbedComponent->SetPhysicsLinearVelocity(FVector(0, 0, 0));
		if (GrabbedComponent->GetOwner()->GetComponentByClass<UPushableProperties>())
		{
			GrabbedComponent->GetOwner()->GetComponentByClass<UPushableProperties>()->NumberOfGrabbers -= 1;
			GrabbedComponent->GetOwner()->GetComponentByClass<UPushableProperties>()->Grabbers.Remove(this); 
		}
		Release();	
	}
}

void UPushComponent::Grab()
{
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

		//Adjust rotation to face grabbed surface
		FRotator NewRotation = HitResult.ImpactNormal.Rotation();
		NewRotation.Pitch = 0.0f;
		NewRotation.Yaw += 180.0f;
		GetOwner()->SetActorRotation(NewRotation);
		
		GetGrabbableInReach(HitResult);
		PhysicsHandle->GrabComponentAtLocation(
			HitComponent,
			NAME_None,
			HitResult.ImpactPoint);
		GrabEffect();
	}
	

}

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

void UPushComponent::UpdateGrabLocation()
{
	if (PhysicsHandle && GrabbedComponent != nullptr)
	{
		Release();
		Grab();
	}
}

