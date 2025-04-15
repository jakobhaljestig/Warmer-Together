#pragma once

#include "CoreMinimal.h"
#include "FWeatherState.generated.h"

USTRUCT(BlueprintType)
struct SPM_API FWeatherState
{
	GENERATED_BODY()

public:
	FWeatherState()
		: Temperature(-10.0f), WindSpeed(10.0f), SnowIntensity(0.5f), Visibility(1.0f), WeatherLevel(1) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather")
	float Temperature;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather")
	float WindSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather")
	float SnowIntensity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather")
	float Visibility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather")
	int32 WeatherLevel;
};
