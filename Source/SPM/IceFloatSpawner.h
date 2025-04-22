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
	
	//USceneComponent för att sätta ut in world, omvandlar till Fvector i cpp-filen 
	UPROPERTY(EditAnywhere, Category = "Spawner")
	USceneComponent* SpawnPosition;

	//Intervall av tid då isflak spawnas 
	UPROPERTY(EditAnywhere)
	float SpawnInterval;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void SpawnIceFloat();

	//Hanterar kopplingen med Delegate, tar emot parameter av det objekt som nåt slutmålet
	UFUNCTION()
	void HandleIceFloatReachedEnd(AMovingIceFloat* IceFloat);
	
	void RespawnIceFloat(AMovingIceFloat* IceFloat);

	AMovingIceFloat* CurrentIceFloat;
	
	FTimerHandle SpawnTimer;

	bool bHasReachedEndPosition = false;

};
