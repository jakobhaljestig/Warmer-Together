

// Fill out your copyright notice in the Description page of Project Settings.


#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlockAgent.h"
#include "FlockManager.generated.h"


UCLASS()
class SPM_API AFlockManager : public AActor
{
	GENERATED_BODY()
  
public:
	// Sets default values for this actor's properties
	AFlockManager();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
  
	TArray<AFlockAgent*> GetNearbyAgents(AFlockAgent* Agent, float Radius) const;
  
	UPROPERTY(EditAnywhere)
	TSubclassOf<AFlockAgent> AgentClass;
  
	UPROPERTY(EditAnywhere)
	int NumAgents = 50;
  
	UPROPERTY(EditAnywhere)
	FVector SpawnBounds = FVector(1000, 1000, 500);
  
private:
	TArray<AFlockAgent*> Agents;
};
