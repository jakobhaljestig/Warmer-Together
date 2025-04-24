#include "AdaptiveWeatherSystem.h"

#include "CharacterBase.h"
#include "EngineUtils.h"
#include "PerformanceTracker.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Engine/ExponentialHeightFog.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

// Konstruktor
UAdaptiveWeatherSystem::UAdaptiveWeatherSystem()
{
	// Kan initialisera standardvärden här
	
}

void UAdaptiveWeatherSystem::BeginPlay()
{

}


void UAdaptiveWeatherSystem::SetCurrentZone(EZoneType NewZone)
{
	CurrentZone = NewZone;
	EvaluatePerformanceAndAdjustWeather(); 
	ApplyEnvironmentEffects();      
		
}

void UAdaptiveWeatherSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	TimeSinceLastUpdate = 0.0f;

	UE_LOG(LogTemp, Warning, TEXT("[AdaptiveWeatherSystem] Initialized"));

	// instatinserar vädret efter 0.5 sekunder, annars hinner inte allt laddas in och snön hittas inte
	if (UWorld* World = GetWorld())
	{
		FTimerHandle TimerHandle;
		World->GetTimerManager().SetTimer(
			TimerHandle,
			this,
			&UAdaptiveWeatherSystem::InitializeEnvironmentReferences,
			0.5f, // Delay i sekunder
			false
		);
	}
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

//läser in exponentianlheightfog och snow niagara-systemet direkt från scenen
void UAdaptiveWeatherSystem::InitializeEnvironmentReferences()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("World is null in InitializeEnvironmentReferences"));
		return;
	}

	for (TActorIterator<AExponentialHeightFog> It(World); It; ++It)
	{
		FogActor = *It;
		UE_LOG(LogTemp, Warning, TEXT("[Weather] Found FogActor: %s"), *FogActor->GetName());
		break;
	}

	// niagarasystemet som finns i scenen (heter NiagaraComponent0, annars hittas ej)
	bool bFoundSnow = false;
	for (TActorIterator<AActor> It(World); It; ++It)
	{
		TArray<UNiagaraComponent*> NiagaraComps;
		It->GetComponents<UNiagaraComponent>(NiagaraComps);
		for (UNiagaraComponent* Comp : NiagaraComps)
		{
			UE_LOG(LogTemp, Warning, TEXT("[Weather] Found Niagara Component: %s"), *Comp->GetName());
			if (Comp->GetName().Contains(TEXT("NiagaraComponent0")))
			{
				SnowParticleSystem = Comp;
				UE_LOG(LogTemp, Warning, TEXT("[Weather] Snow Niagara found: %s"), *Comp->GetName());
				bFoundSnow = true;
				break;
			}
		}
		if (bFoundSnow)
		{
			break;
		}
	}

	if (!bFoundSnow)
	{
		UE_LOG(LogTemp, Error, TEXT("[Weather] SnowParticleSystem not found!"));
	}
}

//påverkar bodytemp baserat på vädernivån, måste nog tweakas lite vart eftersom
void UAdaptiveWeatherSystem::AffectBodyTemperatures() const
{
	UWorld* World = GetWorld();
	if (!World) return;

	int32 WeatherLevel = GetCurrentWeather().WeatherLevel;

	for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (!PC) continue;

		const APawn* Pawn = PC->GetPawn();
		if (!Pawn) continue;

		if (UBodyTemperature* BodyTemp = Pawn->FindComponentByClass<UBodyTemperature>())
		{
			float CoolRate = WeatherLevel * 0.75f; // t.ex. 0.75 → 2.25
			BodyTemp->SetCoolDownRate(CoolRate);

			//UE_LOG(LogTemp, Warning, TEXT("[AffectBodyTemperatures] WeatherLevel=%d → CoolRate=%.2f"),
				//WeatherLevel, CoolRate);
		}
	}
}


//kollar performance score och motifierar zonen däreefter, Vädret går att anpassa, just nu har vi ju bara visability och snöpartiklar (som bara finns som på/av)
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

	AffectBodyTemperatures();

}

//detta bör ersättas med en timer för uppdatering per interval senare då det inte kanske bör uppdateras per frame
void UAdaptiveWeatherSystem::Tick(float DeltaTime)
{
	// uppdaterar tiden som har gått och gör väderuppdatering om det har gått tillräckligt lång tid
	TimeSinceLastUpdate += DeltaTime;

	if (TimeSinceLastUpdate >= UpdateInterval)
	{
		EvaluatePerformanceAndAdjustWeather();
		TimeSinceLastUpdate = 0.0f;
	}
}

//lägger på fog och snow, ytterligare effekter kan läggas till
void UAdaptiveWeatherSystem::ApplyEnvironmentEffects() const
{
		const FWeatherState& Weather = GetCurrentWeather();

		if (!FogActor)
		{
			UE_LOG(LogTemp, Error, TEXT("FogActor is NULL!"));
			return;
		}
		
		UExponentialHeightFogComponent* FogComponent = FogActor->FindComponentByClass<UExponentialHeightFogComponent>();
		if (!FogComponent)
		{
			UE_LOG(LogTemp, Error, TEXT("FogComponent is NULL!"));
		}
		else
		{
			float NewFogDensity = FMath::Clamp(1.0f - Weather.Visibility, 0.1f, 5.0f);
			FogComponent->SetFogDensity(NewFogDensity);
			UE_LOG(LogTemp, Warning, TEXT("Fog updated: %.2f"), NewFogDensity);
		}

		if (SnowParticleSystem)
		{
			//bör kanske inte vara så låg, men någon utträkning blir tokig. När spelaren dött 2 gånger går den under 0.3 och då avaktiveras snön just nu. 
			if (Weather.SnowIntensity > 0.3f)
			{
				SnowParticleSystem->Activate();
				UE_LOG(LogTemp, Warning, TEXT("Snow Activated"));
			}
			else
			{
				SnowParticleSystem->Deactivate();
				UE_LOG(LogTemp, Warning, TEXT("Snow Deactivated"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("SnowParticleSystem is NULL!"));
		}
}







