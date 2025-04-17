// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AIWeatherController.generated.h"

/**
 * 
 */
UCLASS()
class SPM_API AAIWeatherController : public AAIController
{
	GENERATED_BODY()

	public:
	AAIWeatherController();

	protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY()
	UBlackboardComponent* BlackboardComponent;
};

