// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Retreating.generated.h"

/**
 * 
 */
struct FRetreatTaskMemory
{
	FVector StartLocation;
	FVector TargetLocation;
	float ElapsedTime = 0.f;
	float TotalRetreatDuration = 3.f; 
};

UCLASS()
class SPM_API UBTTask_Retreating : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_Retreating();
	uint16 GetInstanceMemorySize() const;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:

	bool bReachedTarget;
};
