// Fill out your copyright notice in the Description page of Project Settings.


#include "BirdAi.h"


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

void ABirdAi::OnPlayerScaredBird()
{
}

void ABirdAi::UpdateCircling(float DeltaTime)
{
	CircleAngle += CircleSpeed * DeltaTime;
	FVector Offset = FVector(FMath::Cos(CircleAngle), FMath::Sin(CircleAngle), 0.f) * CircleRadius;
	SetActorLocation(CircleCenter + Offset + FVector(0, 0, 300)); // fågeln högt upp
}

void ABirdAi::CheckForPlayers()
{
}

void ABirdAi::StartDive(AActor* Player)
{
}

void ABirdAi::UpdateDiving(float DeltaTime)
{
}

void ABirdAi::UpdateRetreating(float DeltaTime)
{
}

