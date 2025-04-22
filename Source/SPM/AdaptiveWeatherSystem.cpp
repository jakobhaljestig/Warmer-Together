#include "AdaptiveWeatherSystem.h"
#include "Engine/World.h"
#include "TimerManager.h"

// Konstruktor
UAdaptiveWeatherSystem::UAdaptiveWeatherSystem()
{
	// Kan initialisera standardvärden här
}

void UAdaptiveWeatherSystem::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("Adaptive Weather System Started"));
}

void UAdaptiveWeatherSystem::SetCurrentZone(EZoneType NewZone)
{
	if (CurrentZone != NewZone)
	{
		CurrentZone = NewZone;

		UE_LOG(LogTemp, Warning, TEXT("Weather Zone Updated to: %d"), static_cast<int32>(NewZone));
		EvaluatePerformanceAndAdjustWeather();
	}
}

// Initialize kallas när subsystemet startas
void UAdaptiveWeatherSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Eventuella initialiseringar av vädersystemet här
	TimeSinceLastUpdate = 0.0f;
}

// Deinitialize kallas när subsystemet tas bort eller stängs av
void UAdaptiveWeatherSystem::Deinitialize()
{
	Super::Deinitialize();

	// Eventuella städoperationer här
}

void UAdaptiveWeatherSystem::UpdatePerformance(const FPerformance& NewPerformance)
{
	CurrentPerformance = NewPerformance;
	EvaluatePerformanceAndAdjustWeather();

	/*UE_LOG(LogTemp, Warning, TEXT("Performance Updated: Deaths=%d, AvgTime=%.1f, TimeNearHeat=%.1f"),
		CurrentPerformance.DeathCount,
		CurrentPerformance.AveragePuzzleTime,
		CurrentPerformance.TimeNearHeat);
		*/
}


const FWeatherState& UAdaptiveWeatherSystem::GetCurrentWeather() const
{
	return CurrentWeather;
}

void UAdaptiveWeatherSystem::EvaluatePerformanceAndAdjustWeather()
{
	float PerformanceScore = CurrentPerformance.RecentPerformanceScore();

	float ZoneModifier = 1.0f;
	switch (CurrentZone)
	{
	case EZoneType::ZONE_NEUTRAL: ZoneModifier = 0.5f; break;
	case EZoneType::ZONE_MEDIUM: ZoneModifier = 1.0f; break;
	case EZoneType::ZONE_INTENSE: ZoneModifier = 1.5f; break;
	}

	float AdjustedScore = FMath::Clamp(PerformanceScore * (1.0f / ZoneModifier), 0.0f, 1.0f);

	// Anpassa vädret
	CurrentWeather.Temperature = FMath::Lerp(-30.0f, 0.0f, AdjustedScore);
	CurrentWeather.WindSpeed = FMath::Lerp(20.0f, 5.0f, AdjustedScore);
	CurrentWeather.SnowIntensity = FMath::Lerp(1.0f, 0.2f, AdjustedScore);
	CurrentWeather.Visibility = FMath::Lerp(0.3f, 1.0f, AdjustedScore);
	CurrentWeather.WeatherLevel = FMath::RoundToInt(FMath::Lerp(3.0f, 1.0f, AdjustedScore));

}

// Detta kan ersättas med en timer för uppdatering per interval senare då det inte kanske bör uppdateras per frame.
void UAdaptiveWeatherSystem::Tick(float DeltaTime)
{
	// Uppdatera tiden som har gått och gör väderuppdatering om det har gått tillräckligt lång tid
	TimeSinceLastUpdate += DeltaTime;

	if (TimeSinceLastUpdate >= UpdateInterval)
	{
		// Kalla på metoder för att uppdatera väder och prestationen
		EvaluatePerformanceAndAdjustWeather();
		TimeSinceLastUpdate = 0.0f;
	}
}





