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
}

void AMovingIceFloat::Tick(float DeltaTime)
{
	MoveIceFloat(DeltaTime);
	
	CheckIfEndReached();
}

void AMovingIceFloat::MoveIceFloat(float DeltaTime)
{
	
}


void AMovingIceFloat::CheckIfEndReached()
{
	//om flak når slutet
	OnReachedEnd.Broadcast();
	Destroy(); //förstör eller bara set to start location 
}