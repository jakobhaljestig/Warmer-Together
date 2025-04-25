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

	SpawnPosition = GetActorLocation();
	
	//UE_LOG(LogTemp, Warning, TEXT("BeginPlay: Spawner started with interval %f"), SpawnInterval);
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
	if (!MovingIceFloatClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("IceFloatClass is empty"))
		return;
	}

	if (InactiveIceFloats.Num() > 0)
	{
		RespawnIceFloat();
	
	}else
	{
		
		FVector SpawnLocation = SpawnPosition;
		FRotator SpawnRotation = FRotator(0, 0, 0);

		AMovingIceFloat* NewFloat = GetWorld() -> SpawnActor<AMovingIceFloat>(MovingIceFloatClass, SpawnLocation, SpawnRotation);
		
		if (NewFloat)
		{
			NewFloat->SetEndTarget(EndTarget->GetActorLocation());
			NewFloat-> SetMovement(MovementSpeed);
			NewFloat->OnReachedEnd.AddDynamic(this, &AIceFloatSpawner::HandleIceFloatReachedEnd);
			ActiveIceFloats.Add(NewFloat);
		}
	}
	
}

void AIceFloatSpawner::HandleIceFloatReachedEnd(AMovingIceFloat* IceFloat)
{
	//UE_LOG(LogTemp, Warning, TEXT("Received delegate - IceFloat has reached the end"))
	IceFloat -> SetActorEnableCollision(false);
	IceFloat -> SetActorHiddenInGame(true);

	ActiveIceFloats.Remove(IceFloat);
	InactiveIceFloats.Add(IceFloat);
	
}

void AIceFloatSpawner::RespawnIceFloat()
{
	if (InactiveIceFloats.Num() == 0) return;

	AMovingIceFloat* IceFloat = InactiveIceFloats[0];
	InactiveIceFloats.RemoveAt(0);

	IceFloat->SetActorLocation(SpawnPosition);
	IceFloat->SetActorRotation(FRotator::ZeroRotator);
	IceFloat->SetActorHiddenInGame(false);
	IceFloat->SetActorEnableCollision(true);

	if (EndTarget)
	{
		IceFloat->SetEndTarget(EndTarget->GetActorLocation());
	}

	IceFloat->SetActorTickEnabled(true);
	
	ActiveIceFloats.Add(IceFloat);

	//UE_LOG(LogTemp, Warning, TEXT("Respawned ice float from pool"));
}
	



