#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_DiveToPlayer.generated.h"
struct FDiveTaskMemory
{
	FVector StartLocation;
	float ElapsedTime = 0.f;
	float TotalDiveDuration = 2.f; 
};

UCLASS(BlueprintType)
class SPM_API UBTTask_DiveToPlayer : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_DiveToPlayer();
	uint16 GetInstanceMemorySize() const;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override {}

	bool bReachedTarget = false;

	UPROPERTY(EditDefaultsOnly, Category = "Bird|Behavior")
	float DiveCompleteDistance = 100.f;
};
