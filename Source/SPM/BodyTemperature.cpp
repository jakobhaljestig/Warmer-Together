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
	
	WeatherSystem = GetWorld()->GetGameInstance()->GetSubsystem<UAdaptiveWeatherSystem>();
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
	if (!bNearHeat)
	{
		float TempEffect = 1.0f;

		if (WeatherSystem)
		{
			const float EnvTemp = WeatherSystem->CachedEnvTemp;

			// Exponentiell ökning för att göra nedkylning snabbare när vädret är riktigt kallt
			if (EnvTemp < 0.0f)
			{
				TempEffect = FMath::GetMappedRangeValueClamped(
					FVector2D(-30.0f, 0.0f),
					FVector2D(3.0f, 0.5f), // Exponentiellt starkare effekt vid kallt väder
					EnvTemp
				);
			}
			else
			{
				TempEffect = FMath::GetMappedRangeValueClamped(
					FVector2D(0.0f, 30.0f),
					FVector2D(0.5f, 0.0f),
					EnvTemp
				);
			}
		}

		float EffectiveCoolRate = CoolDownRate * TempEffect;

		// Här appliceras snabbare nedkylning vid dålig prestation
		Temp -= DeltaTime * EffectiveCoolRate;

		if (Temp < 0.0f) Temp = 0.0f;

		//UE_LOG(LogTemp, Warning, TEXT("Cooling: %.2f | Temp=%.2f | Env=%.2f"),
			//EffectiveCoolRate, Temp, WeatherSystem ? WeatherSystem->GetCurrentWeather().Temperature : -999.0f);
	}

	if (WeatherSystem)
	{
		float EnvTemp = WeatherSystem->CachedEnvTemp;
		//UE_LOG(LogTemp, Warning, TEXT("[BodyTemp] Character %s | EnvTemp: %.2f"), *GetOwner()->GetName(), EnvTemp);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[BodyTemp] Character %s | WeatherSystem is NULL!"), *GetOwner()->GetName());
	}

}


void UBodyTemperature::HeatUp(float DeltaTime)
{
	Temp = Temp + DeltaTime * HeatUpRate;
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

	ACharacter* Char0 = UGameplayStatics::GetPlayerCharacter(this, 0);
	ACharacter* Char1 = UGameplayStatics::GetPlayerCharacter(this, 1);

	UBodyTemperature* Temp0 = Char0 ? Char0->FindComponentByClass<UBodyTemperature>() : nullptr;
	UBodyTemperature* Temp1 = Char1 ? Char1->FindComponentByClass<UBodyTemperature>() : nullptr;

	if (Temp0 && Temp1)
	{
		const float Mean = (Temp0->Temp + Temp1->Temp) / 2.0f;

		if (const float HalfTemp = Temp0->MaxTemp * 0.5f; Mean < HalfTemp)
		{
			float NewTemp = HalfTemp; // höj båda upp till 50%
			UE_LOG(LogTemp, Warning, TEXT("[BodyTemp] Hug boost to 50%% → %.2f"), NewTemp);
			Temp0->Temp = NewTemp;
			Temp1->Temp = NewTemp;
		}
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

	if (!WeatherSystem)
	{
		WeatherSystem = GetWorld()->GetGameInstance()->GetSubsystem<UAdaptiveWeatherSystem>();
		UE_LOG(LogTemp, Warning, TEXT("[BodyTemp] WeatherSystem reinitialized on ResetTemp"));
	}


}

void UBodyTemperature::ModifyTemperature(float DeltaTemperature)
{
	Temp += DeltaTemperature;
	double MinTemp = 0;
	Temp = FMath::Clamp(Temp, MinTemp, MaxTemp);

	UE_LOG(LogTemp, Warning, TEXT("[BodyTemp] Modified Temp by %.1f. New Temp: %.1f (%.1f%%)"), 
		DeltaTemperature, Temp, GetTempPercentage() * 100.0f);
}
