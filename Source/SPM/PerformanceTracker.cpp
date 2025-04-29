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
	
	//bör ej uppdateras i tick men får vara här så länge tills vi har en puzzlemanager
	//Performance.AveragePuzzleTime = TotalPuzzles > 0 ? TotalPuzzleTime / TotalPuzzles : 0.0f;
	TimeSinceLastDeath += DeltaTime;

	if (TimeSinceLastDeath > 60.0f && Performance.DeathCount > 0)
	{
		Performance.DeathCount--;
		TimeSinceLastDeath = 0.0f;
		UE_LOG(LogTemp, Display, TEXT("Forgiveness: Reduced DeathCount to %d"), Performance.DeathCount);
	}
}

void UPerformanceTracker::RegisterDeath()
{
	Performance.DeathCount++;
	TimeSinceLastDeath = 0.0f;
	UE_LOG(LogTemp, Warning, TEXT("[PerformanceTracker] RegisterDeath called: DeathCount = %d on %s"), Performance.DeathCount, *GetOwner()->GetName());
	
}

void UPerformanceTracker::RegisterPuzzleSolved(float TimeToSolve)
{
	TotalPuzzleTime += TimeToSolve;
	TotalPuzzles++;
}

void UPerformanceTracker::RegisterHug()
{
	RecentHugs++;
}

int32 UPerformanceTracker::GetAndResetRecentHugs()
{
	int32 Out =  RecentHugs;
	RecentHugs = 0;
	return Out;
}

