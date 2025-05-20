// Fill out your copyright notice in the Description page of Project Settings.


#include "BodyTemperature.h"
#include "CharacterPlayerController.h"
#include "CharacterSmall.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UBodyTemperature::UBodyTemperature()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Temp = MaxTemp;
}

// Called when the game starts
void UBodyTemperature::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UBodyTemperature::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	ACharacterPlayerController* Controller= Cast<ACharacterPlayerController>(Cast<APawn>(GetOwner())->GetController());
	if (!bNearHeat)
    {
        if (Temp > 0)
        {
        	CoolDown(DeltaTime);
        }
		if (GetTempPercentage() < 0.3 && !bDisplayFreezeEffect && Controller)
		{
			bDisplayFreezeEffect = true;
		}
		if (GetTempPercentage() >= 0.3 && bDisplayFreezeEffect && Controller)
		{
			bDisplayFreezeEffect = false;
		}
		if (Temp == 0)
		{
			if (!bFrozen)
			{
				bFrozen = true;
				HandleFreeze();
			}
		}

    }
	if (bNearHeat && Temp < MaxTemp)
    {
        HeatUp(DeltaTime);
		if (bFrozen)
		{
			bFrozen = false;
			//GetOwner()->GetComponentByClass<UHealth>()->IsFrozen(bFrozen);
		}
    }
}

void UBodyTemperature::IsNearHeat(bool bIsNearHeat)
{
	bNearHeat = bIsNearHeat;
}

void UBodyTemperature::CoolDown(float DeltaTime)
{
	if (!bNearHeat)
	{
		// Använd en fast kylhastighet utan väderpåverkan
		Temp -= DeltaTime * CoolDownRate;

		if (Temp < 0.0f)
		{
			Temp = 0.0f;
		}
	}
}

void UBodyTemperature::HeatUp(float DeltaTime)
{
	Temp = Temp + DeltaTime * HeatUpRate;
	if (Temp > MaxTemp)
	{
		Temp = MaxTemp;
	}
	
	if (GetWorld()->GetTimerManager().IsTimerActive(DeathTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(DeathTimerHandle);
		UE_LOG(LogTemp, Warning, TEXT("[BodyTemp] Recovered from freezing. Death timer cancelled."));
		bHasDied = false;
	}

}


void UBodyTemperature::ShareTemp()
{
	if (bFrozen)
	{
		bFrozen = false;
		//GetOwner()->GetComponentByClass<UHealth>()->IsFrozen(bFrozen);
	}

	ACharacter* Char0 = UGameplayStatics::GetPlayerCharacter(this, 0);
	ACharacter* Char1 = UGameplayStatics::GetPlayerCharacter(this, 1);

	UBodyTemperature* Temp0 = Char0 ? Char0->FindComponentByClass<UBodyTemperature>() : nullptr;
	UBodyTemperature* Temp1 = Char1 ? Char1->FindComponentByClass<UBodyTemperature>() : nullptr;

	if (Temp0 && Temp1)
	{
		/*const float Mean = (Temp0->Temp + Temp1->Temp) / 2.0f;

		if (const float HalfTemp = Temp0->MaxTemp * 0.5f; Mean < HalfTemp)
		{
			float NewTemp = HalfTemp; // höj båda upp till 50%
			UE_LOG(LogTemp, Warning, TEXT("[BodyTemp] Hug boost to 50%% → %.2f"), NewTemp);
			
		}*/
		
		float NewTemp = MaxTemp;
		Temp0->Temp = NewTemp;
		Temp1->Temp = NewTemp;
		UE_LOG(LogTemp, Warning, TEXT("[BodyTemp] Successfully shared temp!"));
		
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[BodyTemp] Missing temperature components for one or both characters."));
	}
}



void UBodyTemperature::ResetTemp()
{
	Temp = MaxTemp;
	bFrozen = false;
}

void UBodyTemperature::ModifyTemperature(float DeltaTemperature)
{
	Temp += DeltaTemperature;
	double MinTemp = 0;
	Temp = FMath::Clamp(Temp, MinTemp, MaxTemp);

	UE_LOG(LogTemp, Warning, TEXT("[BodyTemp] Modified Temp by %.1f. New Temp: %.1f (%.1f%%)"), 
		DeltaTemperature, Temp, GetTempPercentage() * 100.0f);
}

void UBodyTemperature::HandleFreeze()
{
	UE_LOG(LogTemp, Warning, TEXT("[BodyTemp] Player is frozen. Starting death timer..."));

	if (!bHasDied)
	{
		// startar en timer som dödar spelaren efter en delay
		GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, this, &UBodyTemperature::HandleDeath, DeathDelay, false);
	}
}

void UBodyTemperature::HandleDeath()
{
	if (bHasDied) return;
	bHasDied = true;

	ACharacterBase* OwnerChar = Cast<ACharacterBase>(GetOwner());
	if (OwnerChar && !OwnerChar->bHasDied)
	{
		UE_LOG(LogTemp, Warning, TEXT("[BodyTemp] Player has died from cold."));
		OwnerChar->OnDeath(); 
	}
}

void UBodyTemperature::ColdBuff(float CoolDownValue)
{
	Temp -= CoolDownValue;
}


