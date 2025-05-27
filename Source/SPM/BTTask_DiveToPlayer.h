#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_DiveToPlayer.generated.h"

UCLASS(BlueprintType)
class SPM_API UBTTask_DiveToPlayer : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_DiveToPlayer();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override {}

	bool bReachedTarget = false;

	UPROPERTY(EditDefaultsOnly, Category = "Bird|Behavior")
	float DiveCompleteDistance = 100.f;
};
