// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Snowball.h"
#include "TrajectorySpline.h"
#include "ThrowSnowballComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPM_API UThrowSnowballComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UThrowSnowballComponent();

	void Aim();
	
	void Throw();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanThrow = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trajectory")
	TSubclassOf<ATrajectorySpline> TrajectorySplineActorClass;
	
	//TIll ANIMATION
	bool IsAiming() const {return bIsAiming;}

	bool IsThrowing() const {return bIsThrowing;}

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Snowball")
	TSubclassOf<ASnowball> SnowballClass;

	UPROPERTY(EditAnywhere, Category = "Snowball")
	FName HandSocketName = "RightHandSocket";

	UPROPERTY(EditAnywhere, Category = "Snowball")
	float SnowballInterval = 1.0f;
	
	void ResetThrow();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	float Speed = 1500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	float Gravity = 1.0f;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
	bool bIsAiming = false;

	bool bIsThrowing = false;
	
	bool bIsThrowAreaValid; 
	
	FRotator LastAimRotation;

	FVector LastAimLocation;

	FVector LastThrowDirection;
	
	FTimerHandle TimerHandle_ResetThrow;

	void PredictThrowTrajectory();

	void ClearTrajectoryPath();
	
	ATrajectorySpline* TrajectorySplineActor;

	
};

