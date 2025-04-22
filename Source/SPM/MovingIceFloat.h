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

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	USceneComponent* EndReachedPosition;

private:
	//Path för isflaken kommer från spawnern 
	FVector StartLocation;
	FVector EndLocation;

	bool ReachedEnd = false; 
	
	void EndReached();

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MovementSpeed; 
	
};
