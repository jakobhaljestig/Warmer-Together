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
	
}

bool UClimbComponent::CanClimb()
{
	FHitResult Hit;
	return ClimbingInReach(Hit);
}


void UClimbComponent::StartClimb()
{
	bIsClimbing = true;
	//MovementComponent -> SetMovementMode(MOVE_Flying); 
}

void UClimbComponent::StopClimb()
{
	bIsClimbing = false;
	MovementComponent->SetMovementMode(MOVE_Walking);
}


//Ska kolla om objektet framför går att klättra på. 
bool UClimbComponent::ClimbingInReach (FHitResult& HitResult) const
{
	
	if (!ClimbCharacter) return false;

	FVector Start = ClimbCharacter->GetActorLocation();
	FVector ForwardVector = ClimbCharacter->GetActorForwardVector();
	FVector End = Start + ForwardVector * 100.f; //Hur långt karaktären ser framåt, byta ut hårdkodning. 

	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(ClimbCharacter); 
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		//ECC_Visibility kankse inte bra
		ECC_Visibility,
		TraceParams
	);

	if (bHit && HitResult.GetActor() -> ActorHasTag("Climbable"))
	{
		return true;
	}

	return false;
}