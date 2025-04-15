// Fill out your copyright notice in the Description page of Project Settings.


#include "BodyTemperature.h"

#include "CharacterBig.h"
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
	
	if (!bNearHeat)
    {
        if (Temp > 0)
        {
        	CoolDown(DeltaTime);
        }
		if (GetTempPercentage() < 0.3 && !bDisplayFreezeEffect)
		{
			bDisplayFreezeEffect = true;
			Cast<ACharacterPlayerController>(Cast<APawn>(GetOwner())->GetController())->DisplayFreezeEffect(bDisplayFreezeEffect);
		}
		if (GetTempPercentage() >= 0.3 && bDisplayFreezeEffect)
		{
			bDisplayFreezeEffect = false;
			Cast<ACharacterPlayerController>(Cast<APawn>(GetOwner())->GetController())->DisplayFreezeEffect(bDisplayFreezeEffect);
		}
        if (Temp == 0)
        {
        	if (!bFrozen)
        	{
        		bFrozen = true;
        		GetOwner()->GetComponentByClass<UHealth>()->IsFrozen(bFrozen);
        	}
        }
    }
	if (bNearHeat && Temp < MaxTemp)
    {
        HeatUp(DeltaTime);
		if (bFrozen)
		{
			bFrozen = false;
			GetOwner()->GetComponentByClass<UHealth>()->IsFrozen(bFrozen);
		}
    }
}

void UBodyTemperature::IsNearHeat(bool bIsNearHeat)
{
	bNearHeat = bIsNearHeat;
}

void UBodyTemperature::CoolDown(float DeltaTime)
{
	Temp = Temp - DeltaTime * CoolDownRate;
	if (Temp < 0)
	{
		Temp = 0;
	}
}

void UBodyTemperature::HeatUp(float DeltaTime)
{
	Temp = Temp + DeltaTime * CoolDownRate;
	if (Temp > MaxTemp)
	{
		Temp = MaxTemp;
	}
}

void UBodyTemperature::ShareTemp()
{
	if (bFrozen)
	{
		bFrozen = false;
		GetOwner()->GetComponentByClass<UHealth>()->IsFrozen(bFrozen);
	}
	if (TempBig == nullptr || TempSmall == nullptr)
	{
		TempBig = Cast<ACharacterBig>(UGameplayStatics::GetPlayerCharacter(this, 0))->GetComponentByClass<UBodyTemperature>();
		TempSmall = Cast<ACharacterSmall>(UGameplayStatics::GetPlayerCharacter(this, 1))->GetComponentByClass<UBodyTemperature>();
	}
	Temp = (TempBig->Temp + TempSmall->Temp) / 2;
	bHugging = false;
}