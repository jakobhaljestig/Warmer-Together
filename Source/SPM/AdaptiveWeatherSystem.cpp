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

	UE_LOG(LogTemp, Warning, TEXT("Performance Updated: Deaths=%d, AvgTime=%.1f, TimeNearHeat=%.1f"),
		CurrentPerformance.DeathCount,
		CurrentPerformance.AveragePuzzleTime,
		CurrentPerformance.TimeNearHeat);
}


const FWeatherState& UAdaptiveWeatherSystem::GetCurrentWeather() const
{
	return CurrentWeather;
}

void UAdaptiveWeatherSystem::EvaluatePerformanceAndAdjustWeather()
{
	float PerformanceScore = CurrentPerformance.RecentPerformanceScore();
    
	// Logga PerformanceScore för att kontrollera om det varierar som förväntat
	UE_LOG(LogTemp, Warning, TEXT("Performance Score: %.2f"), PerformanceScore);

	CurrentWeather.Temperature = FMath::Lerp(-30.0f, 0.0f, PerformanceScore);
	CurrentWeather.WindSpeed = FMath::Lerp(20.0f, 5.0f, PerformanceScore);
	CurrentWeather.SnowIntensity = FMath::Lerp(1.0f, 0.2f, PerformanceScore);
	CurrentWeather.Visibility = FMath::Lerp(0.3f, 1.0f, PerformanceScore); // Här justeras Visibility
	CurrentWeather.WeatherLevel = FMath::RoundToInt(FMath::Lerp(3.0f, 1.0f, PerformanceScore));

	UE_LOG(LogTemp, Warning, TEXT("Weather Updated: Temp=%.1f, Wind=%.1f, Snow=%.1f, Vis=%.1f, Level=%d"),
		CurrentWeather.Temperature,
		CurrentWeather.WindSpeed,
		CurrentWeather.SnowIntensity,
		CurrentWeather.Visibility,
		CurrentWeather.WeatherLevel);
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





