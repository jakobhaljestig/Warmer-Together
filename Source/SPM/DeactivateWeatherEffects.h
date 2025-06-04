// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "DeactivateWeatherEffects.generated.h"

/**
 * 
 */
UCLASS()
class SPM_API ADeactivateWeatherEffects : public ATriggerBox
{
	GENERATED_BODY()

public:
	ADeactivateWeatherEffects();

protected:
	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);

	AActor* WeatherActor = nullptr;
	
};
