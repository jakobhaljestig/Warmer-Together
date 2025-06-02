// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Snowball.h"
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

	bool IsThrowAreaValid(){return bIsThrowAreaValid;}

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Snowball")
	TSubclassOf<ASnowball> SnowballClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> GroundMarkerClass;

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

	FRotator LastAimRotation;

	FVector LastAimLocation;

	FVector LastThrowDirection;

	AActor* GroundMarker;

	FTimerHandle TimerHandle_ResetThrow;

	float GetSnowballSpeed() const;

	float GetSnowballGravity() const;

	void PredictThrowTrajectory();

	bool bIsThrowAreaValid; 
};

