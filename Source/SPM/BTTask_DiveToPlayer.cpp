#include "BTTask_DiveToPlayer.h"
#include "BirdAi.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "BodyTemperature.h"
#include "CharacterBase.h"

UBTTask_DiveToPlayer::UBTTask_DiveToPlayer()
{
	bNotifyTick = true;
	NodeName = "Dive to Player";
}

EBTNodeResult::Type UBTTask_DiveToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bReachedTarget = false;
	
	return EBTNodeResult::InProgress;
}

void UBTTask_DiveToPlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ABirdAi* Bird = Cast<ABirdAi>(OwnerComp.GetAIOwner()->GetPawn());
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	AActor* Target = Cast<AActor>(BB->GetValueAsObject("TargetPlayer"));
	FVector DiveTarget = BB->GetValueAsVector("DiveTargetLocation");

	if (!Bird || !Target)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
	
	FVector Direction = (DiveTarget - Bird->GetActorLocation()).GetSafeNormal();
	
	Bird->SetActorLocation(Bird->GetActorLocation() + Direction * Bird->DiveSpeed * DeltaSeconds);
	Bird->SetActorRotation(FRotationMatrix::MakeFromX(Direction).Rotator());

	if (FVector::Dist(Bird->GetActorLocation(), DiveTarget) < 100.f)
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
