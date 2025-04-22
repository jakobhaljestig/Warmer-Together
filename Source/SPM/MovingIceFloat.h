// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IceFloat.h"
#include "MovingIceFloat.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReachedEnd);

/**
 * 
 */
UCLASS()
class SPM_API AMovingIceFloat : public AIceFloat
{
	GENERATED_BODY()

public:
	AMovingIceFloat();

	FOnReachedEnd OnReachedEnd;

	virtual void Tick(float DeltaTime) override;

	void MoveIceFloat(float DeltaTime);

protected:
	virtual void BeginPlay() override;

private:
	//Path för isflaken kommer från spawnern 
	FVector StartLocation;
	FVector EndLocation;
	
	void CheckIfEndReached();
	
};
