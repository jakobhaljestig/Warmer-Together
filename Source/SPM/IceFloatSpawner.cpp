// Fill out your copyright notice in the Description page of Project Settings.


#include "IceFloatSpawner.h"

// Sets default values
AIceFloatSpawner::AIceFloatSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AIceFloatSpawner::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &AIceFloatSpawner::SpawnIceFloat, SpawnInterval, true);

	
}

// Called every frame
void AIceFloatSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AIceFloatSpawner::SpawnIceFloat()
{
	
}


