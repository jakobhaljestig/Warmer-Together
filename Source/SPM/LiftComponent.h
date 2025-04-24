// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GrabComponent.h"

#include "LiftComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )


class SPM_API ULiftComponent : public UGrabComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULiftComponent();

	virtual void GrabAndRelease() override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Throw();

	virtual void Lift();
	virtual void Drop(float Force, float VerticalForce);
	
private:
	UPROPERTY(EditAnywhere)
	float ThrowingForce = 750.f;

	UPROPERTY(EditAnywhere)
	float DroppingForce = 350.f;

	UPROPERTY(EditAnywhere)
	float VerticalThrowingForce = 500.f;

	UPROPERTY(EditAnywhere)
	float VerticalDroppingForce = 350.f;
	
	virtual void GrabEffect() override;

	float OriginalMovementSpeed = 0;

	
};


