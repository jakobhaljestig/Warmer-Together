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

		if (GrabbedActor->GetComponentByClass<UPushableProperties>()->bIsFalling || OwnerMovementComponent->IsFalling())
		{
			StopPushing();
		}
		else if(!GrabbedActor->GetComponentByClass<UPushableProperties>()->CanPush()){
			UpdateGrabLocation();
			OwnerMovementComponent->MaxWalkSpeed = 0;
		}
		else if (GrabbedActor->GetComponentByClass<UPushableProperties>()->CanPush())
		{
			OwnerMovementComponent->MaxWalkSpeed = OriginalMovementSpeed/4;
			if (!GetGrabbableInReach(Hit, GrabbedActor->GetComponentByClass<UPushableProperties>()->HoldDistance*1.1))
			{
				//If player is too far away, try to move back into reach, otherwise stop pushing
				int Attempts = 0;
				while (!GetGrabbableInReach(Hit, GrabbedActor->GetComponentByClass<UPushableProperties>()->HoldDistance*1.1) && OwnerMovementComponent->Velocity.Length() > GrabbedComponent->GetComponentVelocity().Length())
				{
					Attempts++;
					Owner->SetActorLocation(Owner->GetActorLocation() + OwnerMovementComponent->GetForwardVector());
					if (Attempts >= 5) break;
				}
				if (!GetGrabbableInReach(Hit))
				{
					StopPushing();
				}
				
			}
			if (GrabbedComponent != nullptr && GrabbedActor->GetComponentByClass<UPushableProperties>()->CanPush())
			{
				FVector TargetLocation =  GrabbedActor->GetComponentByClass<UPushableProperties>()->GetPushPosition() + Owner->GetActorForwardVector() * GrabbedActor->GetComponentByClass<UPushableProperties>()->HoldDistance;
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
		if (GrabbedActor && GrabbedActor->GetComponentByClass<UPushableProperties>())
		{
			GrabbedActor->GetComponentByClass<UPushableProperties>()->NumberOfGrabbers += 1;
			GrabbedActor->GetComponentByClass<UPushableProperties>()->Grabbers.Add(this); 
		}
		
	}

	
}
void UPushComponent::StopPushing()
{
	if (GrabbedComponent != nullptr && Holding)
	{
		GrabbedComponent->SetPhysicsLinearVelocity(FVector(0, 0, 0));
		if (GrabbedActor->GetComponentByClass<UPushableProperties>())
		{
			GrabbedActor->GetComponentByClass<UPushableProperties>()->NumberOfGrabbers -= 1;
			GrabbedActor->GetComponentByClass<UPushableProperties>()->Grabbers.Remove(this); 
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
		Owner->SetActorRotation(NewRotation);
		
		GetGrabbableInReach(HitResult);
		PhysicsHandle->GrabComponentAtLocation(
			HitComponent,
			NAME_None,
			HitResult.ImpactPoint);
		GrabEffect();
		GrabbedComponent = HitComponent;
		GrabbedActor = HitActor;
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
	OwnerMovementComponent->SetPlaneConstraintNormal(Owner->GetActorRightVector() * 200);
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

