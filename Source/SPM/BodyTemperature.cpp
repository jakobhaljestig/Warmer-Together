// Fill out your copyright notice in the Description page of Project Settings.


#include "BodyTemperature.h"

#include "CharacterBig.h"
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

	if (bHugging)
	{
		ShareTemp();
	}
	else
	{
		if (bShouldCoolDown)
        {
        	if (Temp > 0)
        	{
        		CoolDown(DeltaTime);
        		UE_LOG(LogTemp, Warning, TEXT("Temperature cooldown started"));
        	}
        	if (Temp == 0)
        	{
        		// Health down
        		UE_LOG(LogTemp, Warning, TEXT("Temperature is 0"));
        	}
        }
		if (bShouldHeatUp && Temp < MaxTemp)
        {
			UE_LOG(LogTemp, Warning, TEXT("HeatUp"));
         	HeatUp(DeltaTime);
        }
	}
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
	if (TempBig == nullptr || TempSmall == nullptr)
	{
		TempBig = Cast<ACharacterBig>(UGameplayStatics::GetPlayerCharacter(this, 0))->GetComponentByClass<UBodyTemperature>();
		TempSmall = Cast<ACharacterSmall>(UGameplayStatics::GetPlayerCharacter(this, 1))->GetComponentByClass<UBodyTemperature>();
	}
	Temp = (TempBig->Temp + TempSmall->Temp) / 2;
	bHugging = false;
}