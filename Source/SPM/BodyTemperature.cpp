// Fill out your copyright notice in the Description page of Project Settings.


#include "BodyTemperature.h"

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
	if (bShouldCoolDown && Temp > 0)
	{
		CoolDown(DeltaTime);
	}
	if (Temp == 0)
	{
		// Health down
	}
	
}

void UBodyTemperature::CoolDown(float DeltaTime)
{
	Temp = Temp - DeltaTime * CoolDownRate;
}


void UBodyTemperature::HeatUp(float DeltaTime)
{
	Temp = Temp + DeltaTime * CoolDownRate;
}

void UBodyTemperature::ShareTemp()
{
	// Temp = FMath::Lerp(Temp, GetOwner()->OtherPlayer->Temp, ShareTempRate);
}