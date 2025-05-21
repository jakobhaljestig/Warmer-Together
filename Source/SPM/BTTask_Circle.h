#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Circle.generated.h"

UCLASS(BlueprintType)
class SPM_API UBTTask_Circle : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_Circle();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override {}

	float CircleAngle;
};
