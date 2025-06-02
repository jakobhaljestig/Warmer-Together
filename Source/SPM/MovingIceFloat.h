// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IceFloat.h"
#include "MovingIceFloat.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReachedEnd, AMovingIceFloat*, IceFloat);
/**
 * 
 */
UCLASS()
class SPM_API AMovingIceFloat : public AIceFloat
{
	GENERATED_BODY()

public:
	AMovingIceFloat();

	//FOnReachedEnd OnReachedEnd;

	virtual void Tick(float DeltaTime) override;

	void MoveIceFloat(float DeltaTime);

	UPROPERTY(BlueprintAssignable)
	FOnReachedEnd OnReachedEnd;

	void SetEndTarget (FVector InEndLocation);

	void SetMovement (float Speed);

	void SetBreakTime(float InBreakTime);

	void SetRespawnTime(float InRespawnTime);

protected:
	virtual void BeginPlay() override;


private:
	//Path för isflaken kommer från spawnern 
	FVector StartLocation;
	FVector EndLocation;

	//bool ReachedEnd = false; 
	
	void EndReached();

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MovementSpeed; 
	
};
