// Fill out your copyright notice in the Description page of Project Settings.


#include "BirdAi.h"

#include "BTTask_GiveTemporaryBuff.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABirdAi::ABirdAi()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CircleRadius = 500.f;
	CircleSpeed = 1.f;
	CircleAngle = 0.f;
	CurrentState = EBirdState::Circling;

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

	switch (CurrentState)
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
		
	}

}

void ABirdAi::UpdateCircling(float DeltaTime)
{
	CircleAngle += CircleSpeed * DeltaTime;
	FVector Offset = FVector(FMath::Cos(CircleAngle), FMath::Sin(CircleAngle), 0.f) * CircleRadius;
	SetActorLocation(CircleCenter + Offset + FVector(0, 0, 300)); // fågeln högt upp
}

void ABirdAi::CheckForPlayers()
{
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
}

void ABirdAi::StartDive(AActor* Player)
{
	TargetPlayer = Player;
	DiveTargetLocation = Player->GetActorLocation();
	CurrentState = EBirdState::Diving;
}

void ABirdAi::UpdateDiving(float DeltaTime)
{
	FVector CurrentLocation = GetActorLocation();
	FVector Direction = (DiveTargetLocation - CurrentLocation).GetSafeNormal();
	SetActorLocation(CurrentLocation + Direction * DiveSpeed * DeltaTime);

	if (FVector::Dist(CurrentLocation, DiveTargetLocation) < 100.f)
	{
		if (TargetPlayer)
		{
			if (UBodyTemperature* Temp = TargetPlayer -> FindComponentByClass<UBodyTemperature>())
			{
				Temp -> ColdBuff(20.f);
			}
		}
		CurrentState = EBirdState::Retreating;
	}
}

void ABirdAi::UpdateRetreating(float DeltaTime)
{
	FVector Direction = (CircleCenter + FVector(0, 0, 300)) - GetActorLocation();
	SetActorLocation(GetActorLocation() + Direction * DiveSpeed * DeltaTime);

	if (FVector::Dist(GetActorLocation(), CircleCenter + FVector(0, 0, 300)) < 100.f)
	{
		CurrentState = EBirdState::Circling;
	}
}

void ABirdAi::OnPlayerScaredBird()
{
	if (CurrentState == EBirdState::Diving)
	{
		CurrentState = EBirdState::Retreating;
	}
}


