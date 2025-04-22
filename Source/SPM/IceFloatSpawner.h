// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingIceFloat.h"
#include "IceFloatSpawner.generated.h"

UCLASS()
class SPM_API AIceFloatSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIceFloatSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AMovingIceFloat> MovingIceFloatClass;


	//USceneComponent?? 
	UPROPERTY(EditAnywhere)
	FVector SpawnPosition;

	UPROPERTY(EditAnywhere)
	float SpawnInterval;

	UPROPERTY(EditAnywhere)
	int NumberOfIceFloats;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void SpawnIceFloat();
	FTimerHandle SpawnTimer;

};
