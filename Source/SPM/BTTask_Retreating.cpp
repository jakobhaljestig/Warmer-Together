// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Retreating.h"

#include "AIController.h"
#include "BirdAi.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Retreating::UBTTask_Retreating(): bReachedTarget(false)
{
	bNotifyTick = true;
	NodeName = "Retreating";
}

EBTNodeResult::Type UBTTask_Retreating::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bReachedTarget = false;
	return EBTNodeResult::InProgress;
}
void UBTTask_Retreating::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ABirdAi* Bird = Cast<ABirdAi>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Bird)
	{
		UE_LOG(LogTemp, Error, TEXT("[Retreating] Bird pawn is null!"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	// Återställ cirkelbana till original
	Bird->CircleCenter = Bird->OriginalCircleCenter;
	Bird->CircleRadius = Bird->OriginalCircleRadius;

	// räknar ut exakt målpunkt på cirkeln där fågeln ska återuppta sin cirkling
	FVector Offset = FVector(FMath::Cos(Bird->CircleAngle), FMath::Sin(Bird->CircleAngle), 0.f) * Bird->CircleRadius;
	FVector TargetLocation = Bird->CircleCenter + Offset + FVector(0, 0, Bird->CirclingHeight);

	//flyttar fågeln mot mål
	FVector Direction = TargetLocation - Bird->GetActorLocation();
	if (!Direction.IsNearlyZero())
	{
		Direction.Normalize();
		Bird->SetActorLocation(Bird->GetActorLocation() + Direction * Bird->DiveSpeed * DeltaSeconds);
		Bird->SetActorRotation(FRotationMatrix::MakeFromX(Direction).Rotator());
	}

	// när den 'ärtillräckligt nära, avsluta retreat och börja cirkla
	if (FVector::Dist(Bird->GetActorLocation(), TargetLocation) < 100.f)
	{
		Bird->SetActorLocation(TargetLocation); // för säkerhet: sätt exakt

		UE_LOG(LogTemp, Warning, TEXT("[Retreating] Reached final circling point: %s"), *TargetLocation.ToString());

		// markera att nästa cirkling är första tick
		Bird->bFirstTickInCircling = true;

		// sätt status till Circling
		UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
		BBComp->SetValueAsName("BirdStates", "Circling");

		UE_LOG(LogTemp, Display, TEXT("[Retreating] Retreat complete – switching to Circling"));

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}