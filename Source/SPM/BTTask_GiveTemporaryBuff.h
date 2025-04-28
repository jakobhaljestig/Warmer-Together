// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GiveTemporaryBuff.generated.h"

/**
 * 
 */
UCLASS()
class SPM_API UBTTask_GiveTemporaryBuff : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_GiveTemporaryBuff();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	

	static float TemperatureBonus;

	static bool bIsBuffActive;

	UPROPERTY(EditAnywhere, Category = "Buff")
	float BuffDuration = 10.0f;

private:
	static void RevertBuff(ACharacterBase* Player);
	
};
