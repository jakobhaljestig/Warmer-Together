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

uint16 UBTTask_Retreating::GetInstanceMemorySize() const
{
	return sizeof(FRetreatTaskMemory);
}

EBTNodeResult::Type UBTTask_Retreating::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FRetreatTaskMemory* Memory = reinterpret_cast<FRetreatTaskMemory*>(NodeMemory);
	bReachedTarget = false;

	ABirdAi* Bird = Cast<ABirdAi>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Bird)
		return EBTNodeResult::Failed;

	// återställs cirkelns
	Bird->CircleCenter = Bird->OriginalCircleCenter;
	Bird->CircleRadius = Bird->OriginalCircleRadius;

	// Spara startposition
	Memory->StartLocation = Bird->GetActorLocation();

	// Räkna ut och spara exakt målpunkt på cirkeln
	FVector Offset = FVector(FMath::Cos(Bird->CircleAngle), FMath::Sin(Bird->CircleAngle), 0.f) * Bird->CircleRadius;
	Memory->TargetLocation = Bird->CircleCenter + Offset + FVector(0, 0, Bird->CirclingHeight);

	Memory->ElapsedTime = 0.f;
	Memory->TotalRetreatDuration = 4.f; 

	return EBTNodeResult::InProgress;
}


void UBTTask_Retreating::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FRetreatTaskMemory* Memory = (FRetreatTaskMemory*)NodeMemory;
	
	ABirdAi* Bird = Cast<ABirdAi>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Bird)
	{
		UE_LOG(LogTemp, Error, TEXT("[Retreating] Bird pawn is null!"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
	
	// Tidsbaserad rörelse
	Memory->ElapsedTime += DeltaSeconds;
	float Alpha = FMath::Clamp(Memory->ElapsedTime / Memory->TotalRetreatDuration, 0.f, 1.f);
	float SmoothAlpha = FMath::InterpEaseInOut(0.f, 1.f, Alpha, 2.f); // exponent 2 = normal easing

	FVector NewLocation = FMath::Lerp(Memory->StartLocation, Memory->TargetLocation, SmoothAlpha);
	Bird->SetActorLocation(NewLocation);

	// Rotation mot mål
	FVector Direction = (Memory->TargetLocation - NewLocation).GetSafeNormal();
	if (!Direction.IsNearlyZero())
	{
		FRotator DesiredRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
		FRotator NewRotation = FMath::RInterpTo(Bird->GetActorRotation(), DesiredRotation, DeltaSeconds, 5.0f);
		Bird->SetActorRotation(NewRotation);
	}

	// Klar när nära eller alpha = 1.0
	if (FVector::Dist(NewLocation, Memory->TargetLocation) < 100.f || Alpha >= 1.0f)
	{
		Bird->SetActorLocation(Memory->TargetLocation); // exakta målpositionen
		Bird->bFirstTickInCircling = true;

		UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
		BBComp->SetValueAsName("BirdStates", "Circling");

		UE_LOG(LogTemp, Display, TEXT("[Retreating] Retreat complete – switching to Circling"));

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}