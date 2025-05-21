#include "BirdAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BirdAi.h"

void ABirdAIController::BeginPlay()
{
	Super::BeginPlay();

	ABirdAi* BirdPawn = Cast<ABirdAi>(GetPawn());
	if (BirdPawn && BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}
}
