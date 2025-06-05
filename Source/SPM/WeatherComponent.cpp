// Fill out your copyright notice in the Description page of Project Settings.


#include "WeatherComponent.h"

#include "BodyTemperature.h"
#include "NiagaraFunctionLibrary.h"
#include "CharacterBase.h"
#include "Kismet/GameplayStatics.h"



// Sets default values for this component's properties
UWeatherComponent::UWeatherComponent(): SnowLevel1(nullptr), SnowLevel2(nullptr), SnowLevel3(nullptr),
                                        MistParticleSystem(nullptr), SnowSystem1(nullptr), SnowSystem2(nullptr),
                                        SnowSystem3(nullptr),
                                        MistSystem(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWeatherComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	SpawnWeatherEffects();

	if (!bHasSpawnedWeather)
	{
		//SpawnWeatherEffects();
		bHasSpawnedWeather = true;
	}
	
	
}


// Called every frame
void UWeatherComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PlayerCharacters.IsEmpty())
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacterBase::StaticClass(), PlayerCharacters);
	}

	if (bVictory)
	{
		if (SnowLevel1) SnowLevel1->SetWorldLocation(VictoryEffectsLocation);
		if (SnowLevel2) SnowLevel2->SetWorldLocation(VictoryEffectsLocation);
		if (SnowLevel3) SnowLevel3->SetWorldLocation(VictoryEffectsLocation);
		if (MistParticleSystem) MistParticleSystem->SetWorldLocation(VictoryEffectsLocation);
		
		return;
	}
	
	OnWeatherUpdateTick();
	// ...
}

//kan användas i blueprint för att sätta skalan manuellt, ex i cutscenes. 
void UWeatherComponent::SetWeatherEffectScale(FVector NewScale)
{
	if (SnowLevel1) SnowLevel1->SetWorldScale3D(NewScale);
	if (SnowLevel2) SnowLevel2->SetWorldScale3D(NewScale);
	if (SnowLevel3) SnowLevel3->SetWorldScale3D(NewScale);
	if (MistParticleSystem) MistParticleSystem->SetWorldScale3D(NewScale);
}


//spawnar effekterna genom bpn
void UWeatherComponent::SpawnWeatherEffects()
{
	SnowLevel1 = SpawnEffectIfNeeded(SnowSystem1, SnowLevel1, FVector::ZeroVector);
	SnowLevel2 = SpawnEffectIfNeeded(SnowSystem2, SnowLevel2, FVector::ZeroVector);
	SnowLevel3 = SpawnEffectIfNeeded(SnowSystem3, SnowLevel3, FVector::ZeroVector);
	MistParticleSystem = SpawnEffectIfNeeded(MistSystem, MistParticleSystem, FVector::ZeroVector);
}


//om effekt inte finns, spawna
UNiagaraComponent* UWeatherComponent::SpawnEffectIfNeeded(UNiagaraSystem* System, UNiagaraComponent* ExistingComp, FVector Offset) const
{
	if (System && !ExistingComp)
	{
		return UNiagaraFunctionLibrary::SpawnSystemAttached(
			System,
			GetOwner()->GetRootComponent(),
			NAME_None,
			Offset,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			false
		);
	}
	return ExistingComp;
}


void UWeatherComponent::OnWeatherUpdateTick() const
{

	UpdateWeatherEffectLocation();

	float TotalTemp = 0.f;
	int32 Count = 0;

	//hämtar tempen för antalet spelare
	for (AActor* Actor : PlayerCharacters)
	{
		ACharacterBase* Character = Cast<ACharacterBase>(Actor);
		if (!Character) continue;

		UBodyTemperature* TempComp = Character->FindComponentByClass<UBodyTemperature>();
		if (TempComp)
		{
			TotalTemp += TempComp->GetTempPercentage(); // 0.0–1.0
			Count++;
		}
	}

	//räknar ut average temp baserat på antal spelare och uppdaterar vädret därefter
	if (Count > 0)
	{
		float AvgTemp = TotalTemp / Count;
		UpdateWeatherFromTemperature(AvgTemp);
	}
}


void UWeatherComponent::UpdateWeatherFromTemperature(float Temp) const
{
	if (!SnowLevel1 || !SnowLevel2 || !SnowLevel3 || !MistParticleSystem) return;

	// inaktivera allt
	TArray Effects = { SnowLevel1, SnowLevel2, SnowLevel3, MistParticleSystem };
	for (auto* Effect : Effects) if (Effect) Effect->Deactivate();

	EWeatherLevel Level = EWeatherLevel::None;

	//väljer en vädernivå baserat på temp, enum för att göra det mer lättläst och koncist
	if (Temp >= 0.75f)
		Level = EWeatherLevel::Light;
	else if (Temp >= 0.5f)
		Level = EWeatherLevel::Medium;
	else if (Temp >= 0.25f)
		Level = EWeatherLevel::Heavy;

	//aktiverar rätt level
	switch (Level)
	{
	case EWeatherLevel::Light:
		SnowLevel1->Activate();
		break;
	case EWeatherLevel::Medium:
		SnowLevel2->Activate();
		break;
	case EWeatherLevel::Heavy:
		SnowLevel3->Activate();
		MistParticleSystem->Activate();
		break;
	default:
		break;
	}
}


void UWeatherComponent::UpdateWeatherEffectLocation() const
{
	FPlayerSpatialInfo Info = AnalyzePlayerPositions(PlayerCharacters);

	const float ZOffset = 1000.f;
	FVector EffectLocation = FVector(Info.Midpoint.X, Info.Midpoint.Y, Info.AvgZ + ZOffset);

	if (SnowLevel1) SnowLevel1->SetWorldLocation(EffectLocation);
	if (SnowLevel2) SnowLevel2->SetWorldLocation(EffectLocation);
	if (SnowLevel3) SnowLevel3->SetWorldLocation(EffectLocation);
	if (MistParticleSystem) MistParticleSystem->SetWorldLocation(EffectLocation);

	//DrawDebugSphere(GetWorld(), EffectLocation, 50.f, 12, FColor::Green, false, 5.f);
}


//sparar värden i struct som hämtas av updateweathereffectlocaation, förut gjordes detta i både getplayersmidpoint och updateweathereffectlocation
FPlayerSpatialInfo UWeatherComponent::AnalyzePlayerPositions(const TArray<AActor*>& Players)
{
	FPlayerSpatialInfo Result;
	
	if (Players.Num() == 0)
	{
		Result.Midpoint = FVector::ZeroVector;
		Result.AvgZ = 0.f;
		Result.MaxDistance = 0.f;
		return Result;
	}

	FVector TotalLocation = FVector::ZeroVector;
	float MaxDist = 0.f;
	float TotalZ = 0.f;

	//summerar spelarnas positioner och beräknar maxavståndet mellan dem.
	for (int32 i = 0; i < Players.Num(); ++i)
	{
		const FVector& LocA = Players[i]->GetActorLocation();
		TotalLocation += LocA;
		TotalZ += LocA.Z;

		for (int32 j = i + 1; j < Players.Num(); ++j)
		{
			const FVector& LocB = Players[j]->GetActorLocation();
			MaxDist = FMath::Max(MaxDist, FVector::Dist(LocA, LocB));
		}
	}

	int32 Count = Players.Num();
	Result.Midpoint = TotalLocation / Count;
	Result.AvgZ = TotalZ / Count;
	Result.MaxDistance = MaxDist;

	return Result;
}

/*
FVector UWeatherComponent::GetPlayersMidpoint() const
{
    TArray<AActor*> PlayerCharacters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacterBase::StaticClass(), PlayerCharacters);

    if (PlayerCharacters.Num() == 0) return FVector::ZeroVector;

    FVector TotalLocation = FVector::ZeroVector;
    float MaxDistance = 0.0f;

    // Beräkna den genomsnittliga positionen och maxavståndet mellan spelarna
    for (int32 i = 0; i < PlayerCharacters.Num(); ++i)
    {
        AActor* ActorA = PlayerCharacters[i];
        for (int32 j = i + 1; j < PlayerCharacters.Num(); ++j)
        {
            AActor* ActorB = PlayerCharacters[j];
            float Dist = FVector::Dist(ActorA->GetActorLocation(), ActorB->GetActorLocation());
            MaxDistance = FMath::Max(MaxDistance, Dist);
        }

        TotalLocation += ActorA->GetActorLocation();
    }

    // Dela med antalet spelare för att få mittpunkten
    FVector Midpoint = TotalLocation / PlayerCharacters.Num();

    //UE_LOG(LogTemp, Warning, TEXT("[WeatherSystem] Midpoint: X=%.1f Y=%.1f Z=%.1f, MaxDistance: %.1f"), 
        //Midpoint.X, Midpoint.Y, Midpoint.Z, MaxDistance);

    return Midpoint;
}*/

/*
//påverkar bodytemp baserat på vädernivån, måste nog tweakas lite vart eftersom
void UWeatherComponent::AffectBodyTemperatures() const
{
	UWorld* World = GetWorld();
	if (!World) return;
	
	TArray<AActor*> PlayerCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacterBase::StaticClass(), PlayerCharacters);

	int32 Index = 0;
	for (AActor* Actor : PlayerCharacters)
	{
		ACharacterBase* Character = Cast<ACharacterBase>(Actor);
		if (!Character) continue;

		UBodyTemperature* BodyTemp = Character->FindComponentByClass<UBodyTemperature>();
		if (BodyTemp)
		{
			BodyTemp->SetCoolDownRate(CurrentCoolRate);

			/*UE_LOG(LogTemp, Warning, TEXT("[WeatherLog] Character %d - WeatherLevel: %d | CoolDownRate: %.2f | Temp%%: %.1f%%"),
				Index,
				CurrentWeather.WeatherLevel,
				CurrentCoolRate,
				BodyTemp->GetTempPercentage() * 100.0f
			);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[WeatherLog] Character %d - Missing BodyTemperature component!"), Index);
		}

		Index++;
	}
}*/


