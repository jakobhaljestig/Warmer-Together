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
			FinishClimbUp();
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

void UClimbComponent::Climb()
{
	FHitResult Hit;
	if ((!bIsClimbing && ClimbingInReach(Hit)) == true)
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
	
	UArrowComponent* Arrow = Hit.GetActor()->FindComponentByClass<UArrowComponent>();
	if (Arrow)
	{
		//position
		FVector WallDirection = -Arrow->GetForwardVector(); 
		FVector BasePosition = Hit.ImpactPoint;
		FVector AttachPosition = BasePosition + WallDirection * 40.0f;
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
		/*if (bIsOnLedge)
		{
			FinishClimbUp();
			return;
		}*/
		
		FVector NewLocation = ClimbCharacter->GetActorLocation() - ClimbCharacter->GetActorForwardVector() * 20.f;
		ClimbCharacter->SetActorLocation(NewLocation);
		SetWalking();
	}
}

void UClimbComponent::FinishClimbUp()
{
	if (!ClimbCharacter) return;
	
	FVector Forward = ClimbCharacter->GetActorForwardVector();
	FVector Start = ClimbCharacter->GetActorLocation() 
				  + Forward * 150.f            // Lite framför
				  + FVector(0.f, 0.f, 100.f); // Lite upp

	// Raycast rakt ner
	FVector End = Start + FVector(0.f, 0.f, -300.f);

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

	FColor LineColor = bHit ? FColor::Green : FColor::Red;
	DrawDebugLine(GetWorld(), Start, End, LineColor, false, 2.0f, 0, 2.0f);
	
	if (bHit)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Found ledge top at: %s"), *HitResult.ImpactPoint.ToString());

		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 15.f, 12, FColor::Blue, false, 2.0f);
		
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

	

	if (bHit)
	{
		UPrimitiveComponent* Component = HitResult.GetComponent();
		
		if (Component && Component->ComponentHasTag("Climbable"))
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


