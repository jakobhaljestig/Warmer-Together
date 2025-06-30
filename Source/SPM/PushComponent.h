// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LiftComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "PushComponent.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )

class SPM_API UPushComponent : public UGrabComponent
{
	GENERATED_BODY()
	

	public:
	UPushComponent();
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Starts pushing object
	UFUNCTION(BlueprintCallable)
	virtual void StartPushing();

	//Releases object
	UFUNCTION(BlueprintCallable)
	virtual void StopPushing();
	
	virtual void Grab() override;

	private:
	
	virtual void GrabEffect() override;
	
	virtual void ReleaseEffect() override;

	UPROPERTY()
	float OriginalMovementSpeed = 0;
	UPROPERTY()
	FRotator OriginalRotationRate = FRotator(0,0,0);

	//Update grab location to prevent grabbed object from snapping if other player releases it
	void UpdateGrabLocation();

protected:
	virtual void BeginPlay() override;
	void UpdateObjectLocation() const;
	void MaintainRelativeLocation();
};
