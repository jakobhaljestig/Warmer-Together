// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BirdAi.generated.h"

UENUM(BlueprintType)
enum class EBirdState : uint8
{
	Circling,
	Diving,
	Retreating
};

UCLASS()
class SPM_API ABirdAi : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABirdAi();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnPlayerScaredBird();

private:
	EBirdState CurrentState;

	FVector CircleCenter;
	float CircleRadius;
	float CircleSpeed;
	float CircleAngle;

	AActor* TargetPlayer;

	UPROPERTY(EditAnywhere)
	float DetectionRadius = 1000.0f;

	UPROPERTY(EditAnywhere)
	float DiveSpeed = 1000.0f;

	FVector DiveTargetLocation;

	void UpdateCircling(float DeltaTime);
	void CheckForPlayers();
	void StartDive(AActor* Player);
	void UpdateDiving(float DeltaTime);
	void UpdateRetreating(float DeltaTime);

};
