#include "PerformanceTracker.h"
#include "AdaptiveWeatherSystem.h"

UPerformanceTracker::UPerformanceTracker()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPerformanceTracker::BeginPlay()
{
	Super::BeginPlay();

	if (const UGameInstance* GameInstance = GetWorld()->GetGameInstance())
	{
		WeatherUpdater = Cast<IWeatherUpdaterInterface>(GameInstance->GetSubsystem<UAdaptiveWeatherSystem>());
		
		if (!WeatherUpdater)
		{
			UE_LOG(LogTemp, Error, TEXT("WeatherUpdaterInterface not found!"));
		}
	}
}


void UPerformanceTracker::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsNearHeat)
	{
		TimeNearHeat += DeltaTime;
	}

	// uppdatera Performance-structen
	Performance.TimeNearHeat = TimeNearHeat;

	//bör ej uppdateras i tick men får vara här så länge tills vi har en puzzlemanager
	Performance.AveragePuzzleTime = TotalPuzzles > 0 ? TotalPuzzleTime / TotalPuzzles : 0.0f;

	if (WeatherUpdater)
	{
		WeatherUpdater->UpdatePerformance(Performance);
	}
}

void UPerformanceTracker::RegisterDeath()
{
	Performance.DeathCount++;
	UE_LOG(LogTemp, Warning, TEXT("Performance Updated: DeathCount = %d"), Performance.DeathCount);

	// Kontrollera om WeatherUpdater är null
	if (WeatherUpdater)
	{
		WeatherUpdater->UpdatePerformance(Performance);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("WeatherUpdater is null!"));
	}
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
