#include "AdaptiveWeatherSystem.h"
#include "Engine/World.h"
#include "TimerManager.h"

// Konstruktor
UAdaptiveWeatherSystem::UAdaptiveWeatherSystem()
{
	// Kan initialisera standardvärden här
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
}

const FWeatherState& UAdaptiveWeatherSystem::GetCurrentWeather() const
{
	return CurrentWeather;
}

void UAdaptiveWeatherSystem::EvaluatePerformanceAndAdjustWeather()
{
	// Här justeras vädret baserat på prestation
	float PerformanceScore = CurrentPerformance.RecentPerformanceScore();
	// Justera väder på basis av score
	CurrentWeather.Temperature = FMath::Lerp(-30.0f, 0.0f, PerformanceScore);
	CurrentWeather.WindSpeed = FMath::Lerp(20.0f, 5.0f, PerformanceScore);
	CurrentWeather.SnowIntensity = FMath::Lerp(1.0f, 0.2f, PerformanceScore);
	CurrentWeather.Visibility = FMath::Lerp(0.3f, 1.0f, PerformanceScore);
	CurrentWeather.WeatherLevel = FMath::RoundToInt(FMath::Lerp(3.0f, 1.0f, PerformanceScore));
}

// Detta kan ersättas med en timer för uppdatering per interval om du inte vill att det ska ticka varje frame.
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





