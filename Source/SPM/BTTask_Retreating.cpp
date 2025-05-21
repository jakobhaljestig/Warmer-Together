// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Retreating.h"

#include "AIController.h"
#include "BirdAi.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Retreating::UBTTask_Retreating()
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
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	FVector TargetLocation = Bird->CircleCenter + FVector(0,0,500);
	FVector Direction = TargetLocation - Bird->GetActorLocation();

	if (!Direction.IsNearlyZero())
	{
		Direction.Normalize();
		Bird->SetActorLocation(Bird->GetActorLocation() + Direction * Bird->DiveSpeed * DeltaSeconds);
		Bird->SetActorRotation(FRotationMatrix::MakeFromX(Direction).Rotator());
	}

	if (FVector::Dist(Bird->GetActorLocation(), TargetLocation) < 100.f)
	{
		FVector FinalPosition = Bird->GetActorLocation();

		Bird->CircleCenter = FVector(FinalPosition.X, FinalPosition.Y, 0.f);
		Bird->CirclingHeight = FinalPosition.Z;

		FVector ToBird = FVector(FinalPosition.X, FinalPosition.Y, 0.f) - Bird->CircleCenter;

		if (!ToBird.IsNearlyZero())
		{
			ToBird.Normalize();
			Bird->CircleAngle = FMath::Atan2(ToBird.Y, ToBird.X);
		}

		UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
		BBComp -> SetValueAsName("BirdStates", "Circling");

		UE_LOG(LogTemp, Display, TEXT("Reatreating done"));

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
