// Fill out your copyright notice in the Description page of Project Settings.


#include "AIWeatherController.h"

#include "EngineUtils.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "WeatherController.h"


AAIWeatherController::AAIWeatherController()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));

}
void AAIWeatherController::BeginPlay()
{
	Super::BeginPlay();

	// Hitta WeatherController-aktorn i världen (vi antar att det bara finns en)
	AActor* WeatherActor = nullptr;

	for (TActorIterator<AWeatherController> It(GetWorld()); It; ++It)
	{
		WeatherActor = *It;
		break;
	}

	if (!WeatherActor)
	{
		UE_LOG(LogTemp, Error, TEXT("WeatherActor not found!"));
		return;
	}

	if (BlackboardComponent && BehaviorTreeAsset)
	{
		if (UseBlackboard(BehaviorTreeAsset->BlackboardAsset, BlackboardComponent))
		{
			// 🧠 Sätt in WeatherActor i Blackboard
			BlackboardComponent->SetValueAsObject("WeatherActor", WeatherActor);

			RunBehaviorTree(BehaviorTreeAsset);
		}
	}
}

