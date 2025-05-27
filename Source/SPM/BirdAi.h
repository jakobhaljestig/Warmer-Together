// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BirdAi.generated.h"

/*
UENUM(BlueprintType)
enum class EBirdState : uint8
{
	Circling,
	Diving,
	Retreating
};*/

UCLASS()
class SPM_API ABirdAi : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABirdAi();

	UPROPERTY(EditAnywhere)
	float CirclingHeight = 500.f;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnPlayerScaredBird();

	void UpdateCircling(float DeltaTime);
	void CheckForPlayers();
	void StartDive(AActor* Player);
	void UpdateDiving(float DeltaTime);
	void UpdateRetreating(float DeltaTime);

	//EBirdState CurrentState;

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

	float AttackCooldown = 5.0f; // sekunder mellan attacker
	float CooldownTimer = 2.0f;
	bool bCanAttack = true;

	UPROPERTY(VisibleAnywhere)
	class UFloatingPawnMovement* MovementComponent;

	bool bFirstTickInCircling = true;

	FVector OriginalCircleCenter;

	float OriginalCircleRadius;


private:

	


};
