// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LiftComponent.h"
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
	
	virtual void StartPushing();
	virtual void StopPushing();

	private:
	virtual void GrabEffect() override;

	virtual void ReleaseEffect() override;
	
	float OriginalMovementSpeed = 0;
	FRotator OriginalRotationRate = FRotator(0,0,0);

	UPROPERTY(EditAnywhere)
	float MaxPushWeight = 250;
	
	
};
