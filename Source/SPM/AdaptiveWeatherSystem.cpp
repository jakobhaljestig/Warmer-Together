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
UAdaptiveWeatherSystem::UAdaptiveWeatherSystem(): FogActor(nullptr), SnowLevel3(nullptr), SnowLevel2(nullptr),
                                                  SnowLevel1(nullptr),
                                                  MistParticleSystem(nullptr)
{
	// Kan initialisera standardvärden här
}

void UAdaptiveWeatherSystem::BeginPlay()
{
	
}

void UAdaptiveWeatherSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	TimeSinceLastUpdate = 0.0f;

	UE_LOG(LogTemp, Warning, TEXT("[AdaptiveWeatherSystem] Initialized"));

	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UAdaptiveWeatherSystem::OnMapLoaded);

}


// Deinitialize kallas när subsystemet tas bort eller stängs av
void UAdaptiveWeatherSystem::Deinitialize()
{
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
	Super::Deinitialize();

	// Eventuella städoperationer här
}

void UAdaptiveWeatherSystem::OnMapLoaded(UWorld* LoadedWorld)
{
	UE_LOG(LogTemp, Warning, TEXT("[AdaptiveWeatherSystem] Map Loaded: %s"), *LoadedWorld->GetMapName());

	InitializeEnvironmentReferences();

	//ApplyEnvironmentEffects();

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			WeatherUpdateTimerHandle,
			this,
			&UAdaptiveWeatherSystem::OnWeatherUpdateTick,
			5.0f,  // varje 5 sekunder (kan justeras)
			true   // looping
		);

		UE_LOG(LogTemp, Warning, TEXT("[AdaptiveWeatherSystem] Started Weather Update Timer"));
	
	}
}
	

void UAdaptiveWeatherSystem::OnWeatherUpdateTick()
{
	UE_LOG(LogTemp, Warning, TEXT("[AdaptiveWeatherSystem] Weather Tick called"));

	UpdateWeatherEffectLocation();

	// Hämta medeltemperaturen från spelarna
	TArray<AActor*> PlayerCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacterBase::StaticClass(), PlayerCharacters);

	float TotalTemp = 0.f;
	int32 Count = 0;

	for (AActor* Actor : PlayerCharacters)
	{
		ACharacterBase* Character = Cast<ACharacterBase>(Actor);
		if (!Character) continue;

		UBodyTemperature* TempComp = Character->FindComponentByClass<UBodyTemperature>();
		if (TempComp)
		{
			TotalTemp += TempComp->GetTempPercentage(); // 0.0–1.0
			Count++;
		}
	}

	if (Count > 0)
	{
		float AvgTemp = TotalTemp / Count;
		UpdateWeatherFromTemperature(AvgTemp);
	}
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

	// niagarasystemet som finns i scenen
	bool bFoundSnow3 = false;
	bool bFoundSnow2 = false;
	bool bFoundSnow1 = false;
	bool bFoundMist = false;
	for (TActorIterator<AActor> It(World); It; ++It)
	{
		TArray<UNiagaraComponent*> NiagaraComps;
		It->GetComponents<UNiagaraComponent>(NiagaraComps);
		for (UNiagaraComponent* Comp : NiagaraComps)
		{
			UE_LOG(LogTemp, Warning, TEXT("[Weather] Found Niagara Component: %s"), *Comp->GetName());
			if (Comp->GetName().Contains(TEXT("Snow3")))
			{
				SnowLevel3 = Comp;
				UE_LOG(LogTemp, Warning, TEXT("[Weather] Snow Niagara 3 found: %s"), *Comp->GetName());
				bFoundSnow3 = true;
				break;
			}

			if (Comp->GetName().Contains(TEXT("Mist")))
			{
				MistParticleSystem = Comp;
				UE_LOG(LogTemp, Warning, TEXT("[Weather] Mist Niagara found: %s"), *Comp->GetName());
				bFoundMist = true;
				break;
			}

			if (Comp->GetName().Contains(TEXT("Snow2")))
			{
				SnowLevel2 = Comp;
				UE_LOG(LogTemp, Warning, TEXT("[Weather] Snow Niagara 2 found: %s"), *Comp->GetName());
				bFoundSnow2 = true;
				break;
			}

			if (Comp->GetName().Contains(TEXT("Snow1")))
			{
				SnowLevel1 = Comp;
				UE_LOG(LogTemp, Warning, TEXT("[Weather] Snow Niagara 1 found: %s"), *Comp->GetName());
				bFoundSnow1 = true;
				break;
			}
		}
		
		if (bFoundSnow3 && bFoundMist && bFoundSnow2 && bFoundSnow1)
		{
			break;
		}
	}

	if (!bFoundSnow3 && !bFoundSnow2 && !bFoundSnow1)
	{
		UE_LOG(LogTemp, Error, TEXT("[Weather] SnowParticleSystem not found!"));
	}

	if (!bFoundMist)
	{
		UE_LOG(LogTemp, Error, TEXT("[Weather] MistParticleSystem not found!"));
	}
}

void UAdaptiveWeatherSystem::UpdateWeatherFromTemperature(const float TemperaturePercentage) const
{
	if (!SnowLevel1 || !SnowLevel2 || !SnowLevel3 || !MistParticleSystem) return;

	UE_LOG(LogTemp, Warning, TEXT("TempPct: %.2f"), TemperaturePercentage);

	// Allt inaktivt först
	SnowLevel1->Deactivate();
	SnowLevel2->Deactivate();
	SnowLevel3->Deactivate();
	MistParticleSystem->Deactivate();

	if (TemperaturePercentage >= 0.75f)
	{
		SnowLevel1->Activate();
		UE_LOG(LogTemp, Warning, TEXT("Snow 1 Activated"));
	}
	else if (TemperaturePercentage >= 0.5f)
	{
		SnowLevel2->Activate();
		UE_LOG(LogTemp, Warning, TEXT("Snow 2 Activated"));
	}
	else if (TemperaturePercentage >= 0.25f)
	{
		SnowLevel3->Activate();
		MistParticleSystem->Activate();
		UE_LOG(LogTemp, Warning, TEXT("Mist + Snow 3 Activated"));
	}
}

FVector UAdaptiveWeatherSystem::GetPlayersMidpoint() const
{
    TArray<AActor*> PlayerCharacters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacterBase::StaticClass(), PlayerCharacters);

    if (PlayerCharacters.Num() == 0) return FVector::ZeroVector;

    FVector TotalLocation = FVector::ZeroVector;
    float MaxDistance = 0.0f;

    // Beräkna den genomsnittliga positionen och maxavståndet mellan spelarna
    for (int32 i = 0; i < PlayerCharacters.Num(); ++i)
    {
        AActor* ActorA = PlayerCharacters[i];
        for (int32 j = i + 1; j < PlayerCharacters.Num(); ++j)
        {
            AActor* ActorB = PlayerCharacters[j];
            float Dist = FVector::Dist(ActorA->GetActorLocation(), ActorB->GetActorLocation());
            MaxDistance = FMath::Max(MaxDistance, Dist);
        }

        TotalLocation += ActorA->GetActorLocation();
    }

    // Dela med antalet spelare för att få mittpunkten
    FVector Midpoint = TotalLocation / PlayerCharacters.Num();

    UE_LOG(LogTemp, Warning, TEXT("[WeatherSystem] Midpoint: X=%.1f Y=%.1f Z=%.1f, MaxDistance: %.1f"), 
        Midpoint.X, Midpoint.Y, Midpoint.Z, MaxDistance);

    return Midpoint;
}

void UAdaptiveWeatherSystem::UpdateWeatherEffectLocation() const
{
	const FVector Midpoint = GetPlayersMidpoint();

	// Beräkna avståndet mellan spelarna för att bestämma skalan
	TArray<AActor*> PlayerCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacterBase::StaticClass(), PlayerCharacters);

	float MaxDistance = 0.0f;
	for (int32 i = 0; i < PlayerCharacters.Num(); ++i)
	{
		for (int32 j = i + 1; j < PlayerCharacters.Num(); ++j)
		{
			AActor* ActorA = PlayerCharacters[i];
			AActor* ActorB = PlayerCharacters[j];
			float Dist = FVector::Dist(ActorA->GetActorLocation(), ActorB->GetActorLocation());
			MaxDistance = FMath::Max(MaxDistance, Dist);
		}
	}

	// Skala snöeffekterna baserat på avståndet
	float ScaleFactor = FMath::GetMappedRangeValueClamped(FVector2D(0.f, 3000.f), FVector2D(1.0f, 3.0f), MaxDistance);
	// Z-skalan fixeras
	FVector ParticleScale = FVector(ScaleFactor, 1.5f, 2.5f);

	if (MaxDistance > 2500.f)
	{
		if (SnowLevel1) 
		{
			SnowLevel1->SetWorldLocation(Midpoint);
			SnowLevel1->SetWorldScale3D(ParticleScale);
		}
		if (SnowLevel2) 
		{
			SnowLevel2->SetWorldLocation(Midpoint);
			SnowLevel2->SetWorldScale3D(ParticleScale);
		}
		if (SnowLevel3) 
		{
			SnowLevel3->SetWorldLocation(Midpoint);
			SnowLevel3->SetWorldScale3D(ParticleScale);
			UE_LOG(LogTemp, Warning, TEXT("ScaleFactor: %.2f based on MaxDistance: %.1f"), ScaleFactor, MaxDistance);
		}

		if (MistParticleSystem)
		{
			MistParticleSystem->SetWorldLocation(Midpoint);
			MistParticleSystem->SetWorldScale3D(ParticleScale);
		}
	}
}

//påverkar bodytemp baserat på vädernivån, måste nog tweakas lite vart eftersom
void UAdaptiveWeatherSystem::AffectBodyTemperatures() const
{
	UWorld* World = GetWorld();
	if (!World) return;
	
	TArray<AActor*> PlayerCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacterBase::StaticClass(), PlayerCharacters);

	int32 Index = 0;
	for (AActor* Actor : PlayerCharacters)
	{
		ACharacterBase* Character = Cast<ACharacterBase>(Actor);
		if (!Character) continue;

		UBodyTemperature* BodyTemp = Character->FindComponentByClass<UBodyTemperature>();
		if (BodyTemp)
		{
			BodyTemp->SetCoolDownRate(CurrentCoolRate);

			/*UE_LOG(LogTemp, Warning, TEXT("[WeatherLog] Character %d - WeatherLevel: %d | CoolDownRate: %.2f | Temp%%: %.1f%%"),
				Index,
				CurrentWeather.WeatherLevel,
				CurrentCoolRate,
				BodyTemp->GetTempPercentage() * 100.0f
			);*/
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[WeatherLog] Character %d - Missing BodyTemperature component!"), Index);
		}

		Index++;
	}
}

void UAdaptiveWeatherSystem::UpdatePerformance(const FPerformance& Performance)
{
	// Tom implementation – används inte längre
}

/*
void UAdaptiveWeatherSystem::AggregatePerformance()
{
	TArray<AActor*> PlayerCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacterBase::StaticClass(), PlayerCharacters);

	int32 TotalDeaths = 0;
	int32 NumPlayers = 0;

	for (AActor* Actor : PlayerCharacters)
	{
		if (ACharacterBase* Character = Cast<ACharacterBase>(Actor))
		{
			if (UPerformanceTracker* Tracker = Character->FindComponentByClass<UPerformanceTracker>())
			{
				TotalDeaths += Tracker->GetPerformance().DeathCount;
				NumPlayers++;
			}
		}
	}

	if (NumPlayers > 0)
	{
		float AverageDeaths = static_cast<float>(TotalDeaths) / NumPlayers;

		FPerformance AggregatedPerformance;
		AggregatedPerformance.DeathCount = AverageDeaths; // Snitt-dödsantal!

		CurrentPerformance = AggregatedPerformance;
		EvaluatePerformanceAndAdjustWeather();
	}
}

//kollar performance score och motifierar zonen däreefter, Vädret går att anpassa, just nu har vi ju bara visability och snöpartiklar (som bara finns som på/av)
void UAdaptiveWeatherSystem::EvaluatePerformanceAndAdjustWeather()
{
	float PerformanceScore = CurrentPerformance.RecentPerformanceScore();

	float ZoneModifier = 1.0f;
	//Furkan was here
	switch (CurrentZone)
	{
	case EZoneType::ZONE_NEUTRAL: ZoneModifier = 0.5f; break;
	case EZoneType::ZONE_MEDIUM: ZoneModifier = 1.0f; break;
	case EZoneType::ZONE_INTENSE: ZoneModifier = 1.5f; break;
	}

	float AdjustedScore = FMath::Clamp(PerformanceScore * (1.0f / ZoneModifier), 0.0f, 1.0f);

	// Anpassa vädret
	CurrentWeather.Temperature = FMath::Lerp(-30.0f, 0.0f, AdjustedScore);
	CachedEnvTemp = CurrentWeather.Temperature; // Spara så att alla får samma

	CurrentWeather.WindSpeed = FMath::Lerp(20.0f, 5.0f, AdjustedScore);
	CurrentWeather.SnowIntensity = FMath::Lerp(1.0f, 0.2f, AdjustedScore);
	CurrentWeather.Visibility = FMath::Lerp(0.3f, 1.0f, AdjustedScore);
	CurrentWeather.WeatherLevel = FMath::RoundToInt(FMath::Lerp(3.0f, 1.0f, AdjustedScore));

	CurrentCoolRate = CurrentWeather.WeatherLevel * 0.75f;
	AffectBodyTemperatures();

	UE_LOG(LogTemp, Warning, TEXT("[Weather] Zone=%d | AdjustedScore=%.2f | Temp=%.2f"),
	static_cast<int32>(CurrentZone),
	AdjustedScore,
	CurrentWeather.Temperature);


}
void UAdaptiveWeatherSystem::UpdatePerformance(const FPerformance& NewPerformance)
{
	CurrentPerformance = NewPerformance;
	EvaluatePerformanceAndAdjustWeather();

	/*UE_LOG(LogTemp, Warning, TEXT("Performance Updated: Deaths=%d, AvgTime=%.1f, TimeNearHeat=%.1f"),
		CurrentPerformance.DeathCount,
		CurrentPerformance.AveragePuzzleTime,
		CurrentPerformance.TimeNearHeat);
		
}
/lägger på fog och snow, ytterligare effekter kan läggas till
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

		if (SnowLevel3 && MistParticleSystem && SnowLevel2)
		{
			SnowLevel1->Deactivate();
			SnowLevel2->Deactivate();
			SnowLevel3->Deactivate();
			MistParticleSystem->Deactivate();
			//bör kanske inte vara så låg, men någon utträkning blir tokig. När spelaren dött 2 gånger går den under 0.3 och då avaktiveras snön just nu. 
			if (Weather.SnowIntensity > 0.4f)
			{
				SnowLevel3->Activate();
				MistParticleSystem->Activate();
				UE_LOG(LogTemp, Warning, TEXT("Snow 3 Activated"));
			}
			else if (Weather.SnowIntensity > 0.25f)
			{
				SnowLevel2->Activate();
				UE_LOG(LogTemp, Warning, TEXT("Snow 3/Mist Deactivated"));
				UE_LOG(LogTemp, Warning, TEXT("Snow 2 Activated"));
			}
			else
			{
					SnowLevel1->Activate();
					UE_LOG(LogTemp, Warning, TEXT("Snow 3/Mist Deactivated (Low intensity)"));
					UE_LOG(LogTemp, Warning, TEXT("Snow 2 Deactivated"));
					UE_LOG(LogTemp, Warning, TEXT("Snow 1 Activated"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("SnowParticleSystem is NULL!"));
		}
}

void UAdaptiveWeatherSystem::SetCurrentZone(EZoneType NewZone)
{

	if (bIsCooperationDetected && NewZone != EZoneType::ZONE_NEUTRAL)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Weather] Cooperation detected, ignoring zone change."));
		return;
	}
	
	CurrentZone = NewZone;
	EvaluatePerformanceAndAdjustWeather(); 
	ApplyEnvironmentEffects();      
		
}
*/








