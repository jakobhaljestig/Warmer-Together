// Fill out your copyright notice in the Description page of Project Settings.

/*
#include "BTService_CheckHugBehaviour.h"

#include "CharacterBase.h"
#include "PerformanceTracker.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTService_CheckHugBehaviour::UBTService_CheckHugBehaviour()
{
	NodeName = "CheckHugBehaviour";
	Interval = 10.0f;
	RandomDeviation = 1.0f;
	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;

}

void UBTService_CheckHugBehaviour::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UE_LOG(LogTemp, Warning, TEXT("[CheckHugBehaviour] Tick fired"));
 
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	ACharacterBase* Player = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(World, 0));
	if (!Player)
	{
		return;
	}

	UPerformanceTracker* Perf = Player->FindComponentByClass<UPerformanceTracker>();
	if (!Perf)
	{
		return;
	}

	UAdaptiveWeatherSystem* Weather = World->GetGameInstance()->GetSubsystem<UAdaptiveWeatherSystem>();
	if (!Weather)
	{
		return;
	}

	HugAcculuatorTimer += DeltaSeconds;

	if (Perf->GetAndResetRecentHugs() > 0)
	{
		HugsInPeriod += 1;
	}

	if (HugAcculuatorTimer >= HugResetTime)
	{
		UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
		
		if (HugsInPeriod)
		{
			Weather->bIsCooperationDetected = true;
			
			if (BlackboardComp)
			{
				BlackboardComp->SetValueAsBool(TEXT("bIsCooperationDetected"), true); 
			}
			
			Weather->SetCurrentZone(EZoneType::ZONE_NEUTRAL);
			UE_LOG(LogTemp, Warning, TEXT("[WeatherAI] Hug cooperation detected! Milder weather applied."));
		}
		else
		{
			Weather->bIsCooperationDetected = false;
			BlackboardComp->SetValueAsBool(TEXT("bIsCooperationDetected"), false);
			UE_LOG(LogTemp, Warning, TEXT("[WeatherAI] Hugs too few (%d), keeping current zone."), HugsInPeriod);

		}

		HugsInPeriod = 0;
		HugAcculuatorTimer = 0;
	}
}
*/