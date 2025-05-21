// Fill out your copyright notice in the Description page of Project Settings.


#include "BirdAi.h"

#include "BirdAIController.h"

// Sets default values
ABirdAi::ABirdAi()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ABirdAIController::StaticClass();

	//MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
	
	CircleRadius = 500.f;
	CircleSpeed = 1.f;
	CircleAngle = 0.f;
	//CurrentState = EBirdState::Circling;

}

// Called when the game starts or when spawned
void ABirdAi::BeginPlay()
{
	Super::BeginPlay();

	CircleCenter = GetActorLocation(); //startlocation
}

// Called every frame
void ABirdAi::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*switch (CurrentState)
	{
		case EBirdState::Circling:
			UpdateCircling(DeltaTime);
			CheckForPlayers();
			break;

		case EBirdState::Diving:
			UpdateDiving(DeltaTime);
			break;
		case EBirdState::Retreating:
			UpdateRetreating(DeltaTime);
			break;
	}*/
	
	if (!bCanAttack)
	{
		CooldownTimer -= DeltaTime;
		if (CooldownTimer <= 0.f)
		{
			bCanAttack = true;
		}
	}
}

void ABirdAi::UpdateCircling(float DeltaTime)
{
	FVector PreviousLocation = GetActorLocation(); // ← innan ny position

	CircleAngle += CircleSpeed * DeltaTime;
	FVector Offset = FVector(FMath::Cos(CircleAngle), FMath::Sin(CircleAngle), 0.f) * CircleRadius;
	FVector NewLocation = CircleCenter + Offset + FVector(0, 0, CirclingHeight); // ny variabel
	
	SetActorLocation(NewLocation);

	FVector Direction = (NewLocation - PreviousLocation);
	if (!Direction.IsNearlyZero())
	{
		SetActorRotation(FRotationMatrix::MakeFromX(Direction.GetSafeNormal()).Rotator());
	}

	//UE_LOG(LogTemp, Warning, TEXT("Circling at angle: %f"), CircleAngle);
}

/*
void ABirdAi::CheckForPlayers()
{

	if (!bCanAttack)
		return;
	
	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), Players);

	for (AActor* Player : Players)
	{
		float Distance = FVector::Dist(Player->GetActorLocation(), GetActorLocation());
		if (Distance < DetectionRadius)
		{
			StartDive(Player);
			return;
		}
	}
}*/

/*
void ABirdAi::StartDive(AActor* Player)
{
	TargetPlayer = Player;
	DiveTargetLocation = Player->GetActorLocation();
	CurrentState = EBirdState::Diving;
}*/

/*
void ABirdAi::UpdateDiving(float DeltaTime)
{
	FVector CurrentLocation = GetActorLocation();

	FVector Direction = (DiveTargetLocation - CurrentLocation);
	if (!Direction.IsNearlyZero())
	{
		Direction = Direction.GetSafeNormal();
		SetActorLocation(CurrentLocation + Direction * DiveSpeed * DeltaTime);
		SetActorRotation(FRotationMatrix::MakeFromX(Direction).Rotator());

	}


	if (FVector::Dist(CurrentLocation, DiveTargetLocation) < 100.f)
	{
		if (TargetPlayer)
		{
			UBodyTemperature* Temp = TargetPlayer->FindComponentByClass<UBodyTemperature>();
			if (Temp)
			{
				Temp->ColdBuff(20.f);
				
				if (Temp->GetTempPercentage() <= 0.f)
				{
					ACharacterBase* PlayerChar = Cast<ACharacterBase>(TargetPlayer);
					if (PlayerChar)
					{
						PlayerChar->OnDeath();
					}
				}

			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Missing Temp Comp"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("TargetPlayer Nullptr"));
		}

		bCanAttack = false;
		CooldownTimer = AttackCooldown;

		//CurrentState = EBirdState::Retreating;
	}
}*/

/*
void ABirdAi::UpdateRetreating(float DeltaTime)
{
	FVector TargetLocation = CircleCenter + FVector(0, 0, 500);
	FVector Direction = TargetLocation - GetActorLocation();

	if (!Direction.IsNearlyZero())
	{
		Direction = Direction.GetSafeNormal();
		SetActorLocation(GetActorLocation() + Direction * DiveSpeed * DeltaTime);
		SetActorRotation(FRotationMatrix::MakeFromX(Direction).Rotator());

		if (FVector::Dist(GetActorLocation(), TargetLocation) < 100.f)
		{
			FVector FinalPosition = GetActorLocation();

			CircleCenter = FVector(FinalPosition.X, FinalPosition.Y, 0.f);
			CirclingHeight = FinalPosition.Z;

			FVector ToBird = FVector(FinalPosition.X, FinalPosition.Y, 0.f) - CircleCenter;
			ToBird.Normalize();

			if (!ToBird.IsNearlyZero())
			{
				ToBird.Normalize();
				CircleAngle = FMath::Atan2(ToBird.Y, ToBird.X);
			}

			//UE_LOG(LogTemp, Warning, TEXT("Resetting CircleAngle from position: %s"), *GetActorLocation().ToString());
			UE_LOG(LogTemp, Warning, TEXT("circling again."));
			
			//CurrentState = EBirdState::Circling;
		}

	}

}

void ABirdAi::OnPlayerScaredBird()
{
	if (CurrentState == EBirdState::Diving)
	{
		CurrentState = EBirdState::Retreating;
	}
}
*/



