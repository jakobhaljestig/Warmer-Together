// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FPerformance.h"
#include "WeatherUpdaterInterface.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI)
class UWeatherUpdaterInterface : public UInterface
{
	GENERATED_BODY()
	
};

class SPM_API IWeatherUpdaterInterface
{
	GENERATED_BODY()

public:
	virtual void UpdatePerformance(const FPerformance& Performance) = 0;
};
