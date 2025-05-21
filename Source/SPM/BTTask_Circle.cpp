#include "BTTask_Circle.h"
#include "BirdAi.h"
#include "AIController.h"

UBTTask_Circle::UBTTask_Circle()
{
	bNotifyTick = true;
	NodeName = "Circle Movement";
}

EBTNodeResult::Type UBTTask_Circle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::InProgress;
}

void UBTTask_Circle::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	ABirdAi* Bird = Cast<ABirdAi>(AICon->GetPawn());
	if (!Bird)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	Bird->UpdateCircling(DeltaSeconds);
}
