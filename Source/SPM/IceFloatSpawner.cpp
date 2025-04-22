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

//Spawnar in isflak tills första isfalket når slutmålet 
void AIceFloatSpawner::SpawnIceFloat()
{
	if (!MovingIceFloatClass || !SpawnPosition)
	{
		return;
	}

	if (!bHasReachedEndPosition)
	{
		FVector SpawnLocation = SpawnPosition->GetComponentLocation();
		FRotator SpawnRotation = FRotator(0, 0, 0);

		CurrentIceFloat = GetWorld() -> SpawnActor<AMovingIceFloat>(MovingIceFloatClass, SpawnLocation, SpawnRotation);
		
		if (CurrentIceFloat)
		{
			CurrentIceFloat->OnReachedEnd.AddDynamic(this, &AIceFloatSpawner::HandleIceFloatReachedEnd);
		}
	}else
	{
		RespawnIceFloat(CurrentIceFloat);
	}
	
}

void AIceFloatSpawner::HandleIceFloatReachedEnd(AMovingIceFloat* IceFloat)
{
	UE_LOG(LogTemp, Warning, TEXT("Received delegate - IceFloat has reached the end"))
	bHasReachedEndPosition = true;

	IceFloat -> SetActorEnableCollision(false);
	IceFloat -> SetActorHiddenInGame(true);
	//Måste troligtvis stoppa movement 
}

void AIceFloatSpawner::RespawnIceFloat(AMovingIceFloat* IceFloat)
{
	
}
	



