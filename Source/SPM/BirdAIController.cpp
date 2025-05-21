#include "BirdAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BirdAi.h"
#include "BehaviorTree/BlackboardComponent.h"


ABirdAIController::ABirdAIController(): BlackboardComp(nullptr), BehaviorComp(nullptr)
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(
		TEXT("/Script/AIModule.BehaviorTree'/Game/AI/BT_BirdAi.BT_BirdAi'"));
	if (BT.Succeeded())
	{
		BehaviorTreeAsset = BT.Object;
	}
}

void ABirdAIController::BeginPlay()
{
	Super::BeginPlay();

	//ABirdAi* BirdPawn = Cast<ABirdAi>(GetPawn());
	/*
	if (BirdPawn && BehaviorTreeAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("BirdAIController: Running BT"));
		RunBehaviorTree(BehaviorTreeAsset);
	}*/
	

}

void ABirdAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTreeAsset && BehaviorTreeAsset->BlackboardAsset)
	{
		if (UseBlackboard(BehaviorTreeAsset->BlackboardAsset, BlackboardComp))
		{
			BlackboardComp->SetValueAsName("BirdStates", "Circling");
			RunBehaviorTree(BehaviorTreeAsset);

			UE_LOG(LogTemp, Warning, TEXT("BirdStates set to: %s"), *BlackboardComp->GetValueAsName("BirdStates").ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BT or Blackboard is missing in OnPossess!"));
	}
}

