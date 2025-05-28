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
	
	// === Flocking ===
	void ApplyFlocking(float DeltaTime);
	FVector Cohesion();
	FVector Separation();
	FVector Alignment();
	FVector StayInBounds() const;
	FVector AvoidObstacles() const;
	FVector GetDynamicWander(float Time) const;

	// === Movement ===
	FVector Velocity;

	UPROPERTY(EditAnywhere, Category = "Flocking")
	float MaxSpeed = 400.f;

	UPROPERTY(EditAnywhere, Category = "Flocking")
	float MaxForce = 100.f;

	UPROPERTY(EditAnywhere, Category = "Flocking")
	float NeighbourRadius = 500.f;

	UPROPERTY(EditAnywhere, Category = "Flocking|Weights")
	float CohesionWeight = 1.f;

	UPROPERTY(EditAnywhere, Category = "Flocking|Weights")
	float SeparationWeight = 1.5f;

	UPROPERTY(EditAnywhere, Category = "Flocking|Weights")
	float AlignmentWeight = 1.f;

	UPROPERTY(EditAnywhere, Category = "Flocking|Weights")
	float RandomWeight = 0.5f;

	// === Per-agent variation ===
	float UniqueOffset;

	float PersonalMaxSpeed;
	float PersonalMaxForce;
	float PersonalCohesionWeight;
	float PersonalSeparationWeight;
	float PersonalAlignmentWeight;

	float DynamicNeighbourRadius;


	// === base värden ===
	float BaseSpeed = MaxSpeed;
	float BaseForce = MaxForce;
	
};
