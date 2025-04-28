// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GiveTemporaryBuff.h"

#include "Kismet/GameplayStatics.h"

float UBTTask_GiveTemporaryBuff::TemperatureBonus = 10.0f;
bool UBTTask_GiveTemporaryBuff::bIsBuffActive = false;

UBTTask_GiveTemporaryBuff::UBTTask_GiveTemporaryBuff()
{
	NodeName = "Give Temporary Buff";
}

EBTNodeResult::Type UBTTask_GiveTemporaryBuff::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacterBase* Player = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (!Player)
	{
		return EBTNodeResult::Failed;
	}

	UBodyTemperature* TempComp = Player->FindComponentByClass<UBodyTemperature>();
	if (!TempComp)
	{
		return EBTNodeResult::Failed;
	}
	
	if (bIsBuffActive)
	{
		//UE_LOG(LogTemp, Warning, TEXT("[AI] Buff is already active."));
		return EBTNodeResult::Succeeded; // om buffen redan är aktiv, gör inget
	}

	// Buffa spelarens temperatur
	TempComp->ModifyTemperature(TemperatureBonus);
	bIsBuffActive = true;  // Sätt buffen som aktiv

	// Starta timer för att ta bort buffen efter BuffDuration sekunder
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDel;
	TimerDel.BindLambda([this, Player]()
	{
		RevertBuff(Player);
	});

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, BuffDuration, false);

	UE_LOG(LogTemp, Warning, TEXT("[AI] Gave temporary warmth buff of %.1f degrees for %.1f seconds!"), TemperatureBonus, BuffDuration);

	return EBTNodeResult::Succeeded;
}

void UBTTask_GiveTemporaryBuff::RevertBuff(ACharacterBase* Player)
{
	if (!Player) return;

	UBodyTemperature* TempComp = Player->FindComponentByClass<UBodyTemperature>();
	
	 
	if (TempComp)
	{
		// Återställ genom att ta bort samma mängd buff
		TempComp->ModifyTemperature(-TemperatureBonus);

		// Återställ buffstatus
		bIsBuffActive = false;

		UE_LOG(LogTemp, Warning, TEXT("[AI] Reverted temporary warmth buff of %.1f degrees."), TemperatureBonus);
	}
}
