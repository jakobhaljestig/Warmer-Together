// Fill out your copyright notice in the Description page of Project Settings.


#include "ClimbComponent.h"

#include "CharacterSmall.h"
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
	
}

//Metoden är rätt värdelös
void UClimbComponent::Climb()
{

	FHitResult Hit;
	if (!bIsClimbing && ClimbingInReach(Hit))
	{
		StartClimb(Hit);
	}
	else if (bIsClimbing)
	{
		StopClimb();
	}
	/*else if (bIsOnLedge && bIsClimbing)
	{
		FinishClimbUp();
	}*/
	
}


void UClimbComponent::StartClimb(FHitResult Hit)
{
	//UE_LOG(LogTemplateCharacter, Display, TEXT("Player Small is Climbing"));
		
	//IMpactPoint där trace channel träffar objekt
	FVector AttachNormal = Hit.ImpactNormal;
	FVector AttachPosition = Hit.ImpactPoint + AttachNormal * 20.f; //Kanske fixA, ja..ja fixa
	ClimbCharacter->SetActorLocation(AttachPosition);

	//ROTATION
	FRotator WallRotation = Hit.ImpactNormal.ToOrientationRotator();
	WallRotation.Yaw += 180.f;
	FRotator FinalRotation = FRotator(0.f, WallRotation.Yaw, 0.f);
	ClimbCharacter->SetActorRotation(FinalRotation);
		
	bIsClimbing = true;
	MovementComponent -> SetMovementMode(MOVE_Flying); //ANtar att gravity blir 0
	MovementComponent->GravityScale = 0.0f;
	MovementComponent->MaxFlySpeed = 200.f;
	MovementComponent->BrakingDecelerationFlying = 5000.f;
	MovementComponent->bOrientRotationToMovement = false;
	
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

		bIsClimbing = false;
		MovementComponent->SetMovementMode(MOVE_Walking);
		MovementComponent->GravityScale = 1.0f;
		MovementComponent->MaxWalkSpeed = 500.f;
		MovementComponent->BrakingDecelerationWalking = 2048.f;
		MovementComponent->bOrientRotationToMovement = true;
	}
}


void UClimbComponent::FinishClimbUp()
{
	if (!ClimbCharacter) return;

	UE_LOG(LogTemp, Display, TEXT("FinishClimbUp"));
	FVector Start = ClimbCharacter->GetActorLocation() + FVector(0.f, 0.f, 150.f); //raycast från över  huvuedt
	FVector Forward = ClimbCharacter->GetActorForwardVector();
	FVector Direction = (Forward + FVector(0.f, 0.f, -1.5f)).GetSafeNormal(); 
	FVector End = Start + Direction * 150.f; 

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

	DrawDebugLine(GetWorld(),
		Start,
		End,
		FColor::Green,
		false,
		2.0f,
		0,
		2.0f);


	if (bHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found ledge top at: %s"), *HitResult.ImpactPoint.ToString());
		
		FVector TargetLocation = HitResult.ImpactPoint + FVector(0.f, 0.f, 50.f);
		ClimbCharacter->SetActorLocation(TargetLocation);
	}

	// Återställ movement
	bIsClimbing = false;
	bIsOnLedge = false;

	MovementComponent->SetMovementMode(MOVE_Walking);
	MovementComponent->GravityScale = 1.0f;
	MovementComponent->bOrientRotationToMovement = true;
}




//Ska kolla om objektet framför går att klättra på. 
bool UClimbComponent::ClimbingInReach (FHitResult& HitResult) const
{
	if (!ClimbCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("Climb component is null"));
		return false;
	}

	FVector Start = ClimbCharacter->GetActorLocation()  + FVector(0, 0, 80.f); //Flyttar upp linetrace mer mot huvudet
	FVector ForwardVector = ClimbCharacter->GetActorForwardVector();
	FVector End = Start + ForwardVector * 100.f; //Hur långt karaktären ser framåt, byta ut hårdkodning. 

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
	
	if (bHit && HitResult.GetActor() -> ActorHasTag("Climbable"))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Raycast hitting climbing object"));
		return true;
	}
	
	return false;
}
