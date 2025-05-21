#include "BTTask_Circle.h"
#include "BirdAi.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Circle::UBTTask_Circle()
{
	bNotifyTick = true;
	NodeName = "Circle Movement";
}

EBTNodeResult::Type UBTTask_Circle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Warning, TEXT("BTTask_Circle: Executing"));
	return EBTNodeResult::InProgress; 
}

void UBTTask_Circle::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ABirdAi* Bird = Cast<ABirdAi>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Bird)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	FName State = OwnerComp.GetBlackboardComponent()->GetValueAsName("BirdStates");
	
	if (State != "Circling")
	{
		UE_LOG(LogTemp, Warning, TEXT("BTTask_Circle: Ending due to BirdState != Circling"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("BTTask_Circle: Ticking"));
	Bird->UpdateCircling(DeltaSeconds);
}
