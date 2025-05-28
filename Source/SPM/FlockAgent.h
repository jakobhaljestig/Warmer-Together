// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlockAgent.generated.h"

class AFlockManager;

UCLASS()
class SPM_API AFlockAgent : public AActor
{
	GENERATED_BODY()
  
public:
	// Sets default values for this actor's properties
	AFlockAgent();
	void Initialize(AFlockManager* Manager);


	virtual FVector GetVelocity() const override { return Velocity;}


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere)
	AFlockManager* FlockManager;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SkeletalMesh;

	
	FVector Velocity;

	UPROPERTY(EditAnywhere)
	float MaxSpeed = 600.f;
	UPROPERTY(EditAnywhere)
	float MaxForce = 200.f;
	UPROPERTY(EditAnywhere)
	float NeighbourRadius = 500.f;;

	FVector Alignment();
  
	FVector Cohesion();
  
	FVector Separation();

	FVector AvoidObstacles() const;

	FVector StayInBounds() const;
	
	void ApplyFlocking(float DeltaTime);
	
};
