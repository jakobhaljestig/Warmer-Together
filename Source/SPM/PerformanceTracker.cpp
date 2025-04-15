#include "PerformanceTracker.h"
#include "AdaptiveWeatherSystem.h"

UPerformanceTracker::UPerformanceTracker()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPerformanceTracker::BeginPlay()
{
	Super::BeginPlay();
}

void UPerformanceTracker::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsNearHeat)
	{
		TimeNearHeat += DeltaTime;
	}

	// Uppdatera Performance-structen
	Performance.TimeNearHeat = TimeNearHeat;
	Performance.AveragePuzzleTime = TotalPuzzles > 0 ? TotalPuzzleTime / TotalPuzzles : 0.0f;

	if (WeatherSystem)
	{
		WeatherSystem->UpdatePerformance(Performance);
	}
}

void UPerformanceTracker::RegisterDeath()
{
	Performance.DeathCount++;
}

void UPerformanceTracker::RegisterPuzzleSolved(float TimeToSolve)
{
	TotalPuzzleTime += TimeToSolve;
	TotalPuzzles++;
}

void UPerformanceTracker::SetIsNearHeat(bool bNear)
{
	bIsNearHeat = bNear;
}
