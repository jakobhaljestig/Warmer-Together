// Fill out your copyright notice in the Description page of Project Settings.


#include "AdaptiveWeatherSystem.h"

// Sets default values for this component's properties
UAdaptiveWeatherSystem::UAdaptiveWeatherSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAdaptiveWeatherSystem::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAdaptiveWeatherSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TimeSinceLastUpdate += DeltaTime;
	if (TimeSinceLastUpdate >= UpdateInterval)
	{
		EvaluatePerformanceAndAdjustWeather();
		TimeSinceLastUpdate = 0.0f;
	}
}

void UAdaptiveWeatherSystem::UpdatePerformance(const FPerformance& NewPerformance)
{
	CurrentPerformance = NewPerformance;
}

const FWeatherState& UAdaptiveWeatherSystem::GetCurrentWeather() const
{
	return CurrentWeather;
}

void UAdaptiveWeatherSystem::EvaluatePerformanceAndAdjustWeather()
{
	float Score = CurrentPerformance.RecentPerformanceScore();

	// Anpassa vädret efter prestation
	CurrentWeather.Temperature = FMath::Lerp(-30.0f, 0.0f, Score); //dålig prestation = -30 C, bra = 0
	CurrentWeather.WindSpeed   = FMath::Lerp(20.0f, 5.0f, Score); //dålig stark vind 20m/s, bra lugn vind 5m/s
	CurrentWeather.SnowIntensity = FMath::Lerp(1.0f, 0.2f, Score); //dålig mycket snö, bra lite snö
	CurrentWeather.Visibility = FMath::Lerp(0.3f, 1.0f, Score); //dålig låg sikt (vfx dimma osv), bra god sikt
	CurrentWeather.WeatherLevel = FMath::RoundToInt(FMath::Lerp(3.0f, 1.0f, Score)); //konverterar vädret till nivå ex. 3 == storm, 1 == lugnt. Avrundas till int 1,2,3 beroende på hur bra spelaren spelar.

	/*
	*if (playerDeathsLast10Min > threshold || puzzleFailRateHigh)
	*{
	makeWeatherEasier();
	}
	else if (playersDoingWell)
	{
	increaseWeatherChallenge();
	}
	 */
}




