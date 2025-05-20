// Fill out your copyright notice in the Description page of Project Settings.


#include "ClimbComponent.h"
#include "Components/BoxComponent.h"
#include "CharacterSmall.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UClimbComponent::UClimbComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UClimbComponent::BeginPlay()
{
	Super::BeginPlay();
	ClimbCharacter = Cast<ACharacter>(GetOwner());
	if (ClimbCharacter)
	{
		MovementComponent = ClimbCharacter->GetCharacterMovement();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Climb component is null"));
	}
}


// Called every frame
void UClimbComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsClimbing)
	{
		FHitResult Hit;
		if (!ClimbingInReach(Hit))
		{
			bIsOnLedge = true;
		}else
		{
			bIsOnLedge = false;
		}
	}

	if (MovementComponent && bIsClimbing)
	{
		FVector Velocity = MovementComponent->Velocity;
		Velocity.X = 0.f;
		Velocity.Y = 0.f;
		MovementComponent->Velocity = Velocity;
	}
}

//Metoden är rätt värdelös
void UClimbComponent::Climb()
{
	FHitResult Hit;
	if ((!bIsClimbing && ClimbingInReach(Hit)) == true /*|| (!bIsClimbing && ClimbingDownInReach(Hit))*/)
	{
		StartClimb(Hit);
	}
	else if (bIsClimbing)
	{
		StopClimb();
	}
}

void UClimbComponent::StartClimb(FHitResult Hit)
{
	bIsClimbing = true;
	
	///Gammal implementation
	/*FVector AttachNormal = Hit.ImpactNormal;
	FVector AttachPosition = Hit.ImpactPoint + AttachNormal * 10.f; 
	ClimbCharacter->SetActorLocation(AttachPosition);*/

	//ROTATION
	UArrowComponent* Arrow = Hit.GetActor()->FindComponentByClass<UArrowComponent>();
	if (Arrow)
	{
		//position
		FVector WallDirection = -Arrow->GetForwardVector(); 
		FVector BasePosition = Hit.ImpactPoint;
		FVector AttachPosition = BasePosition + WallDirection * 10.0f;
		ClimbCharacter->SetActorLocation(AttachPosition);

		//Gör så spelaren roteras åt samma håll som pilen
		FRotator ArrowRotation = Arrow->GetComponentRotation();
		FRotator NewRotation = FRotator(0.f, ArrowRotation.Yaw, 0.f);
		ClimbCharacter->SetActorRotation(NewRotation);
	}

	SetClimbingMovement();
}


void UClimbComponent::StopClimb()
{
	if (bIsClimbing)
	{
		if (bIsOnLedge)
		{
			FinishClimbUp();
			return;
		}
		
		FVector NewLocation = ClimbCharacter->GetActorLocation() - ClimbCharacter->GetActorForwardVector() * 20.f;
		ClimbCharacter->SetActorLocation(NewLocation);
		SetWalking();
	}
}


void UClimbComponent::FinishClimbUp()
{
	if (!ClimbCharacter) return;
	
	FVector Start = ClimbCharacter->GetActorLocation() + FVector(0.f, 0.f, 150.f); //raycast från över  huvuedt
	FVector Forward = ClimbCharacter->GetActorForwardVector();
	FVector Direction = (Forward + FVector(0.f, 0.f, -1.5f)).GetSafeNormal(); 
	FVector End = Start + Direction * 300.f; 

	FHitResult HitResult;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(ClimbCharacter);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		TraceParams
	);
	
	if (bHit)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Found ledge top at: %s"), *HitResult.ImpactPoint.ToString());
		
		FVector TargetLocation = HitResult.ImpactPoint + FVector(0.f, 0.f, 50.f);
		ClimbCharacter->SetActorLocation(TargetLocation);
	}
	
	bIsOnLedge = false;
	SetWalking();
}

bool UClimbComponent::ClimbingInReach(FHitResult& HitResult) const
{
	if (!ClimbCharacter) return false;

	FVector Start = ClimbCharacter->GetActorLocation() + FVector(0, 0, 80.f);
	FVector Forward = ClimbCharacter->GetActorForwardVector();
	FVector End = Start + Forward * 100.f;

	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(ClimbCharacter);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_GameTraceChannel3,
		TraceParams
	);

	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.5f, 0, 2.0f);

	if (bHit && HitResult.GetActor()->ActorHasTag("Climbable"))
	{
		TArray<UActorComponent*> Components = HitResult.GetActor()->GetComponentsByTag(UBoxComponent::StaticClass(), "ClimbZone");
		for (UActorComponent* Comp : Components)
		{
			UBoxComponent* Box = Cast<UBoxComponent>(Comp);
			if (Box && Box->IsOverlappingActor(ClimbCharacter))
			{
				return true;
			}
		}
	}
	return false;
}



bool UClimbComponent::ClimbingDownInReach(FHitResult& HitResult) const
{
	if (!ClimbCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("Climb component is null"));
		return false;
	}

	FVector Forward = ClimbCharacter->GetActorForwardVector();
	FVector Start = ClimbCharacter->GetActorLocation() + Forward * 150.f + FVector(0.f, 0.f, -50.f);
	
	FVector Direction = (-Forward + FVector(0.f, 0.f, -1.f)).GetSafeNormal();
	FVector End = Start + Direction * 200.f;

	FVector HalfSize(20.f, 20.f, 30.f);
	
	FQuat Rotation = FQuat::Identity;

	FVector MidPoint = (Start + End) * 0.5f;
	DrawDebugBox(GetWorld(), MidPoint, HalfSize, Rotation, FColor::Cyan, false, 1.5f);
	DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 1.5f, 0, 1.0f);
	
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(ClimbCharacter);

	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		Start,
		End,
		Rotation,
		ECC_GameTraceChannel3,
		FCollisionShape::MakeBox(HalfSize),
		TraceParams
	);
	
	if (bHit && HitResult.GetActor()->ActorHasTag("Climbable"))
	{
		TArray<UActorComponent*> Components = HitResult.GetActor()->GetComponentsByTag(UBoxComponent::StaticClass(), "ClimbDownZone");
		for (UActorComponent* Comp : Components)
		{
			UBoxComponent* Box = Cast<UBoxComponent>(Comp);
			if (Box && Box->IsOverlappingActor(ClimbCharacter))
			{
				return true;
			}
		}
		return false;
	}
	return false;
}

void UClimbComponent::SetClimbingMovement()
{
	MovementComponent -> SetMovementMode(MOVE_Flying); //Antar att gravity blir 0
	MovementComponent->GravityScale = 0.0f;
	MovementComponent->MaxFlySpeed = 400.f;
	MovementComponent->BrakingDecelerationFlying = 5000.f;
	MovementComponent->bOrientRotationToMovement = false;
}

void UClimbComponent::SetWalking()
{
	bIsClimbing = false;
	MovementComponent->SetMovementMode(MOVE_Walking);
	MovementComponent->GravityScale = 1.75f;
	MovementComponent->MaxWalkSpeed = 500.f;
	MovementComponent->BrakingDecelerationWalking = 2048.f;
	MovementComponent->bOrientRotationToMovement = true;
}

bool UClimbComponent::ClimbTargetStillValid(FHitResult& HitResult) const
{
	if (!ClimbCharacter) return false;

	FVector Start = ClimbCharacter->GetActorLocation() + FVector(0.f, 0.f, 80.f);
	FVector Forward = ClimbCharacter->GetActorForwardVector();
	FVector End = Start + Forward * 200.f;
	FVector HalfSize(30.f, 30.f, 50.f);
	FQuat Rotation = FQuat::Identity;

	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(ClimbCharacter);

	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		Start,
		End,
		Rotation,
		ECC_GameTraceChannel3,
		FCollisionShape::MakeBox(HalfSize),
		TraceParams
	);

	if (bHit && HitResult.GetActor()->ActorHasTag("Climbable"))
	{
		TArray<UActorComponent*> Components = HitResult.GetActor()->GetComponentsByTag(UBoxComponent::StaticClass(), "ClimbZone");
		for (UActorComponent* Comp : Components)
		{
			UBoxComponent* Box = Cast<UBoxComponent>(Comp);
			if (Box && Box->IsOverlappingActor(ClimbCharacter))
			{
				return true;
			}
		}
	}
	return false;
}

