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


	AFlockManager* FlockManager;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* Mesh;
  
	FVector Velocity;
	
	float MaxSpeed = 600.f;
	float MaxForce = 200.f;
	float NeighbourRadius = 500.f;

	FVector Alignement();
  
	FVector Cohesion();
  
	FVector Separation();
	
	void ApplyFlocking(float DeltaTime);
	
};
