#include "BTTask_DiveToPlayer.h"
#include "BirdAi.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "BodyTemperature.h"
#include "CharacterBase.h"

UBTTask_DiveToPlayer::UBTTask_DiveToPlayer()
{
	bNotifyTick = true;
	NodeName = "Dive to Player";
}

uint16 UBTTask_DiveToPlayer::GetInstanceMemorySize() const
{
	return sizeof(FDiveTaskMemory);
}


EBTNodeResult::Type UBTTask_DiveToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FDiveTaskMemory* Memory = reinterpret_cast<FDiveTaskMemory*>(NodeMemory);
	
	if (const ABirdAi* Bird = Cast<ABirdAi>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		Memory->StartLocation = Bird->GetActorLocation();
	}
	
	Memory->ElapsedTime = 0.f;
	Memory->TotalDiveDuration = 3.f;

	return EBTNodeResult::InProgress;
}

void UBTTask_DiveToPlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FDiveTaskMemory* Memory = reinterpret_cast<FDiveTaskMemory*>(NodeMemory);
	
	ABirdAi* Bird = Cast<ABirdAi>(OwnerComp.GetAIOwner()->GetPawn());
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	
	if (!Bird || !BB )
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	AActor* Target = Cast<AActor>(BB->GetValueAsObject("TargetPlayer"));
	FVector DiveTarget = BB->GetValueAsVector("DiveTargetLocation");

	if (!Target)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	//interpolering med mjuk start/slut
	Memory->ElapsedTime += DeltaSeconds;
	float Alpha = FMath::Clamp(Memory->ElapsedTime / Memory->TotalDiveDuration, 0.f, 1.f);
	
	float SmoothAlpha = FMath::InterpEaseInOut(0.f, 1.f, Alpha, 2.f); //justera för mer/mindre easing
	FVector NewLocation = FMath::Lerp(Memory->StartLocation, DiveTarget, SmoothAlpha);
	
	Bird->MoveSmoothlyTo(Memory->StartLocation, DiveTarget, SmoothAlpha);
	const FVector Direction = (DiveTarget - Bird->GetActorLocation()).GetSafeNormal();
	Bird->RotateSmoothlyTowards(Direction, DeltaSeconds, 3.f);

	float Distance = FVector::Dist(NewLocation, DiveTarget);
	if (Distance < DiveCompleteDistance || Alpha >= 1.0f)
	{
		UBodyTemperature* Temp = Target->FindComponentByClass<UBodyTemperature>();
		if (Temp)
		{
			Temp->ColdBuff(20.f);
			if (Temp->GetTempPercentage() <= 0.f)
			{
				if (ACharacterBase* PlayerChar = Cast<ACharacterBase>(Target))
				{
					PlayerChar->OnDeath();
				}
			}
		}


		Bird->bCanAttack = false;
		Bird->CooldownTimer = Bird->AttackCooldown;

		BB->SetValueAsName("BirdStates", "Retreating");

		UE_LOG(LogTemp, Warning, TEXT("Dive Task: Executing"));

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
