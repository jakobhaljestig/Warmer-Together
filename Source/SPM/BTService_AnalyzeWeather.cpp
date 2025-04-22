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
	Interval = 5.0f;
	RandomDeviation = 1.0f;
	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = false;
	
}
void UBTService_AnalyzeWeather::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UE_LOG(LogTemp, Warning, TEXT("[AnalyzeWeather] Tick fired"));

	ACharacterBase* PlayerChar = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!PlayerChar) return;

	UPerformanceTracker* PerfTracker = PlayerChar->FindComponentByClass<UPerformanceTracker>();
	if (!PerfTracker) return;

	const FPerformance& Perf = PerfTracker->GetPerformance();

	UWorld* World = PlayerChar->GetWorld();
	if (!World) return;

	UAdaptiveWeatherSystem* WeatherSystem = World->GetGameInstance()->GetSubsystem<UAdaptiveWeatherSystem>();
	if (!WeatherSystem) return;

	// --- Zonlogik: baserad på prestation ---
	EZoneType NewZone = EZoneType::ZONE_MEDIUM;

	if (Perf.DeathCount >= 4 || Perf.AveragePuzzleTime > 45.0f)
	{
		NewZone = EZoneType::ZONE_NEUTRAL; // Milt
	}
	else if (Perf.AveragePuzzleTime < 20.0f && Perf.DeathCount == 0 && Perf.TimeNearHeat > 60.0f)
	{
		NewZone = EZoneType::ZONE_INTENSE; // Hårdare
	}

	UE_LOG(LogTemp, Warning, TEXT("[BTService_AnalyzeWeather] Set zone to %d (Deaths=%d, AvgTime=%.1f, HeatTime=%.1f)"),
		static_cast<int32>(NewZone), Perf.DeathCount, Perf.AveragePuzzleTime, Perf.TimeNearHeat);

	WeatherSystem->SetCurrentZone(NewZone);
	
}

