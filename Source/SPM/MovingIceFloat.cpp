// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingIceFloat.h"



AMovingIceFloat::AMovingIceFloat()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMovingIceFloat::BeginPlay()
{
	Super::BeginPlay();
	//Tar location från där spawner sätter location in world 
	//StartLocation = GetActorLocation();
}

void AMovingIceFloat::Tick(float DeltaTime)
{
	MoveIceFloat(DeltaTime);
}

void AMovingIceFloat::MoveIceFloat(float DeltaTime)
{
	FVector CurrentLocation = GetActorLocation();

	FVector Direction = (EndLocation - CurrentLocation).GetSafeNormal();
	FVector NewLocation = CurrentLocation + Direction * MovementSpeed * DeltaTime;

	SetActorLocation(NewLocation);

	if (FVector::Dist(NewLocation, EndLocation) < 10.f) 
	{
		EndReached(); 
	}
}


void AMovingIceFloat::EndReached()
{
	OnReachedEnd.Broadcast(this);
	SetActorTickEnabled(false);
	UE_LOG(LogTemp, Warning, TEXT("EndLocation Reached"));
}

void AMovingIceFloat::SetEndTarget(FVector InEndLocation)
{
	StartLocation = GetActorLocation();
	EndLocation = InEndLocation;
	UE_LOG(LogTemp, Warning, TEXT("%s received end location: %s"), *GetName(), *EndLocation.ToString());
}

void AMovingIceFloat::SetMovement (float Speed)
{
	MovementSpeed = Speed;
}