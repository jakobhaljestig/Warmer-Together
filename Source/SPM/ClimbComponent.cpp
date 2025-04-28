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
	//Kommentera ut behövs inte tick sen när input läggs till
	//CanClimb();
	
}

//Metoden är rätt värdelös
bool UClimbComponent::CanClimb()
{
	FHitResult Hit;
	return ClimbingInReach(Hit);
}


void UClimbComponent::StartClimb()
{
	
	FHitResult Hit;
	if (ClimbingInReach(Hit))
	{
		UE_LOG(LogTemplateCharacter, Display, TEXT("Player Small is Climbing"));
		/*
		//IMpactPoint där trace channel träffar objekt 
		FVector AttachPosition = Hit.ImpactPoint; //Kanske fixA
		ClimbCharacter->SetActorLocation(AttachPosition);

		//Rotation relevant? troligtvis måste ändå karaktären kolla mot väggen
		FRotator ClimbRotation = Hit.ImpactPoint.ToOrientationRotator();
		ClimbRotation.Yaw += 180.f;
		ClimbCharacter->SetActorRotation(ClimbRotation);
		
		bIsClimbing = true;
		MovementComponent -> SetMovementMode(MOVE_Flying); //ANtar att gravity blir 0*/
	}

}

void UClimbComponent::StopClimb()
{
	if (bIsClimbing)
	{
		bIsClimbing = false;
		MovementComponent->SetMovementMode(MOVE_Walking);
	}
	
}


//Ska kolla om objektet framför går att klättra på. 
bool UClimbComponent::ClimbingInReach (FHitResult& HitResult) const
{
	if (!ClimbCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("Climb component is null"));
		return false;
	}

	FVector Start = ClimbCharacter->GetActorLocation();
	FVector ForwardVector = ClimbCharacter->GetActorForwardVector();
	FVector End = Start + ForwardVector * 100.f; //Hur långt karaktären ser framåt, byta ut hårdkodning. 

	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(ClimbCharacter);

	DrawDebugLine(
	   GetWorld(),
	   Start,
	   End,
	   FColor::Red, 
	   false,       
	   5.0f,        
	   0,           
	   5.f        
   );

	//Använder den mest banala funktionelitet kommer se skit ut i spelet. 
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_GameTraceChannel3,
		TraceParams
	);
	
	if (bHit && HitResult.GetActor() -> ActorHasTag("Climbable"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Raycast hitting climbing object"));
		return true;
	}
	
	return false;
}