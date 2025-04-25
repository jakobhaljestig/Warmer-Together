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

	FVector SpawnPosition;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AMovingIceFloat> MovingIceFloatClass;

	UPROPERTY(EditInstanceOnly, Category = "Spawner")
	AActor* EndTarget;


	//Intervall av tid då isflak spawnas 
	UPROPERTY(EditAnywhere)
	float SpawnInterval = 1.0f;

	UPROPERTY(EditAnywhere)
	TArray <AMovingIceFloat*> ActiveIceFloats;

	UPROPERTY(EditAnywhere)
	TArray <AMovingIceFloat*> InactiveIceFloats;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void SpawnIceFloat();

	//Hanterar kopplingen med Delegate, tar emot parameter av det objekt som nåt slutmålet
	UFUNCTION()
	void HandleIceFloatReachedEnd(AMovingIceFloat* IceFloat);
	
	void RespawnIceFloat();
	
	FTimerHandle SpawnTimer;

	bool bHasReachedEndPosition = false;

};
