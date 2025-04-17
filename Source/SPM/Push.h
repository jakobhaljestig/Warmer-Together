// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "PushPull.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPM_API UPush : public UPickup
{
	GENERATED_BODY()
	

	public:
	UPush();
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GrabAndRelease() override;
	protected:
	virtual void StartPushing();
	virtual void StopPushing();
	
};
