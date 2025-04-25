// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_AnalyzeWeather.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PerformanceTracker.h"
#include "AdaptiveWeatherSystem.h"
#include "CharacterBase.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

UBTService_AnalyzeWeather::UBTService_AnalyzeWeather()
{
	NodeName = "BTService_AnalyzeWeather";
	Interval = 10.0f;
	RandomDeviation = 1.0f;
	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;
	
}
void UBTService_AnalyzeWeather::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UE_LOG(LogTemp, Warning, TEXT("[AnalyzeWeather] Tick fired"));

	ACharacterBase* PlayerChar = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!PlayerChar) return;

	//hittar performance tracker på characters som komponent. 
	UPerformanceTracker* PerfTracker = PlayerChar->FindComponentByClass<UPerformanceTracker>();
	if (!PerfTracker) return;

	//hämtar performancetrackern från FPerformance, den som uppdaterar spelarens statistik
	const FPerformance& Perf = PerfTracker->GetPerformance();

	/*
	FPerformance Perf;
	Perf.DeathCount = 10; // många dödsfall
	Perf.AveragePuzzleTime = 100; // lång tid */

	UWorld* World = PlayerChar->GetWorld();
	if (!World) return;


	//läser in väder subsystemet
	UAdaptiveWeatherSystem* WeatherSystem = World->GetGameInstance()->GetSubsystem<UAdaptiveWeatherSystem>();
	if (!WeatherSystem) return;
	
	// --- Zonlogik: baserad på prestation ---
	EZoneType NewZone = EZoneType::ZONE_MEDIUM;

	//itereras efter vi har mer statistik, just nu kollar den bara deathcount. 
	if (Perf.DeathCount >= 2)
	{
		NewZone = EZoneType::ZONE_NEUTRAL; // milt
	}
	else if (Perf.DeathCount == 0)
	{
		NewZone = EZoneType::ZONE_INTENSE; // hårt (snö och dimma)
	}

	UE_LOG(LogTemp, Warning, TEXT("[BTService_AnalyzeWeather] Set zone to %d (Deaths=%d)"),
		static_cast<int32>(NewZone), Perf.DeathCount);

	WeatherSystem->SetCurrentZone(NewZone);
	WeatherSystem->UpdatePerformance(Perf);       // sätter vädret baserat på dålig prestation (just nu bara deathcount)
	WeatherSystem->ApplyEnvironmentEffects();     // visuell uppdatering

	UE_LOG(LogTemp, Warning, TEXT("[TEST] Simulated BAD PERFORMANCE: Snow=%.2f Visibility=%.2f Temp=%.1f"),
		WeatherSystem->GetCurrentWeather().SnowIntensity,
		WeatherSystem->GetCurrentWeather().Visibility,
		WeatherSystem->GetCurrentWeather().Temperature);

	
}

