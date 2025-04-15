// Fill out your copyright notice in the Description page of Project Settings.


#include "IceFloat.h"

// Sets default values
AIceFloat::AIceFloat()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AIceFloat::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AIceFloat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Ska göra bättre hantering av breaking sen 
void AIceFloat::BreakObject()
{
	UE_LOG(LogTemp, Warning, TEXT("Breaking Object"));
	Timer.Start(GetWorld(), BreakTime, [this]()->void
	{
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);

		Timer.Start(GetWorld(), RespawnTime, [this]()->void
			{
				RespawnObject();
			});
	});
}

void AIceFloat::RespawnObject()
{
	UE_LOG(LogTemp, Warning, TEXT("Respawning Object"));
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}

