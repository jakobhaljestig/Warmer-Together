#include "BTService_CheckForPlayers.h"
#include "AIController.h"
#include "BirdAi.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_CheckForPlayers::UBTService_CheckForPlayers()
{
	NodeName = "Check for Players";
	Interval = 0.5f;
}

void UBTService_CheckForPlayers::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	ABirdAi* Bird = Cast<ABirdAi>(AICon->GetPawn());

	UE_LOG(LogTemp, Warning, TEXT("Checking for players"));

	if (!Bird || !Bird->bCanAttack)
		return;

	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsOfClass(Bird->GetWorld(), ACharacter::StaticClass(), Players);

	for (AActor* Player : Players)
	{
		float Distance = FVector::Dist(Player->GetActorLocation(), Bird->GetActorLocation());
		if (Distance < Bird->DetectionRadius)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject("TargetPlayer", Player);
			OwnerComp.GetBlackboardComponent()->SetValueAsVector("DiveTargetLocation", Player->GetActorLocation());
			OwnerComp.GetBlackboardComponent()->SetValueAsName("BirdStates", "Diving");
			return;
		}
	}
}
