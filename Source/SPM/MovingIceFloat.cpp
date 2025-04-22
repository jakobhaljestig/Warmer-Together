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
	StartLocation = GetActorLocation();
	EndLocation = EndReachedPosition->GetComponentLocation();
}

void AMovingIceFloat::Tick(float DeltaTime)
{
	MoveIceFloat(DeltaTime);
}

void AMovingIceFloat::MoveIceFloat(float DeltaTime)
{
	FVector CurrentLocation =  StartLocation;
	/*Fixa flak movement till end location
	 * if()
	 * EndReached();
	 */
}


void AMovingIceFloat::EndReached()
{
	OnReachedEnd.Broadcast(this);
	UE_LOG(LogTemp, Warning, TEXT("EndLocation Reached"));
}