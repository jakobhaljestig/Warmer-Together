// Fill out your copyright notice in the Description page of Project Settings.


#include "WeatherComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "CharacterBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UWeatherComponent::UWeatherComponent(): SnowLevel3(nullptr), SnowLevel2(nullptr), SnowLevel1(nullptr),
                                        MistParticleSystem(nullptr)
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

	OnWeatherUpdateTick();

	// ...
}

void UWeatherComponent::SetWeatherEffectScale(FVector NewScale)
{
	if (SnowLevel1) SnowLevel1->SetWorldScale3D(NewScale);
	if (SnowLevel2) SnowLevel2->SetWorldScale3D(NewScale);
	if (SnowLevel3) SnowLevel3->SetWorldScale3D(NewScale);
	if (MistParticleSystem) MistParticleSystem->SetWorldScale3D(NewScale);
}


void UWeatherComponent::SpawnWeatherEffects()
{
	if (SnowSystem1 && !SnowLevel1)
	{
		SnowLevel1 = UNiagaraFunctionLibrary::SpawnSystemAttached(
			SnowSystem1,
			GetOwner()->GetRootComponent(),
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			false
		);
	}

	if (SnowSystem2 && !SnowLevel2)
	{
		SnowLevel2 = UNiagaraFunctionLibrary::SpawnSystemAttached(
			SnowSystem2,
			GetOwner()->GetRootComponent(),
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			false
		);
	}

	if (SnowSystem3 && !SnowLevel3)
	{
		SnowLevel3 = UNiagaraFunctionLibrary::SpawnSystemAttached(
			SnowSystem3,
			GetOwner()->GetRootComponent(),
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			false
		);
	}

	if (MistSystem && !MistParticleSystem)
	{
		MistParticleSystem = UNiagaraFunctionLibrary::SpawnSystemAttached(
			MistSystem,
			GetOwner()->GetRootComponent(),
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			false
		);
	}
}

	

void UWeatherComponent::OnWeatherUpdateTick() 
{

	UpdateWeatherEffectLocation();

	// Hämta medeltemperaturen från spelarna
	TArray<AActor*> PlayerCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacterBase::StaticClass(), PlayerCharacters);

	float TotalTemp = 0.f;
	int32 Count = 0;

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

	if (Count > 0)
	{
		float AvgTemp = TotalTemp / Count;
		UpdateWeatherFromTemperature(AvgTemp);
	}
}


void UWeatherComponent::UpdateWeatherFromTemperature(const float TemperaturePercentage) 
{
	if (!SnowLevel1 || !SnowLevel2 || !SnowLevel3 || !MistParticleSystem) return;
	
	// Allt inaktivt först
	SnowLevel1->Deactivate();
	SnowLevel2->Deactivate();
	SnowLevel3->Deactivate();
	MistParticleSystem->Deactivate();
	//TemperaturePrecent = 1-TemperaturePercentage;
	if (TemperaturePercentage >= 0.75f)
	{
		SnowLevel1->Activate();
		//UE_LOG(LogTemp, Warning, TEXT("Snow 1 Activated"));
	}
	else if (TemperaturePercentage >= 0.5f)
	{
		SnowLevel2->Activate();
		//UE_LOG(LogTemp, Warning, TEXT("Snow 2 Activated"));
	}
	else if (TemperaturePercentage >= 0.25f)
	{
		SnowLevel3->Activate();
		MistParticleSystem->Activate();
		//UE_LOG(LogTemp, Warning, TEXT("Mist + Snow 3 Activated"));
	}
}

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
}

void UWeatherComponent::UpdateWeatherEffectLocation() const
{
	const FVector Midpoint = GetPlayersMidpoint();
	const float ZOffset = 1000.f;

	// Beräkna avståndet mellan spelarna för att bestämma skalan
	TArray<AActor*> PlayerCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacterBase::StaticClass(), PlayerCharacters);

	float MaxDistance = 0.0f;
	for (int32 i = 0; i < PlayerCharacters.Num(); ++i)
	{
		for (int32 j = i + 1; j < PlayerCharacters.Num(); ++j)
		{
			AActor* ActorA = PlayerCharacters[i];
			AActor* ActorB = PlayerCharacters[j];
			float Dist = FVector::Dist(ActorA->GetActorLocation(), ActorB->GetActorLocation());
			MaxDistance = FMath::Max(MaxDistance, Dist);
		}
	}

	// Skala snöeffekterna baserat på avståndet
	float ScaleFactor = FMath::GetMappedRangeValueClamped(FVector2D(0.f, 3000.f), FVector2D(1.0f, 3.0f), MaxDistance);
	// Z-skalan fixeras
	FVector ParticleScale = FVector(ScaleFactor, 1.5f, 2.5f);

	float AvgZ = 0.f;
	for (AActor* Actor : PlayerCharacters)
	{
		AvgZ += Actor->GetActorLocation().Z;
	}
	AvgZ /= PlayerCharacters.Num();

	FVector EffectLocation = FVector(Midpoint.X, Midpoint.Y, AvgZ + ZOffset);
	
		if (SnowLevel1) 
		{
			SnowLevel1->SetWorldLocation(EffectLocation);
			if (MaxDistance > 5000.f)
			{
				SnowLevel1->SetWorldScale3D(ParticleScale);
			}
		}
		if (SnowLevel2) 
		{
			SnowLevel2->SetWorldLocation(EffectLocation);
			//SnowLevel2->SetWorldScale3D(ParticleScale);
			if (MaxDistance > 5000.f)
			{
				SnowLevel2->SetWorldScale3D(ParticleScale);
			}
		}
		if (SnowLevel3) 
		{
			SnowLevel3->SetWorldLocation(EffectLocation);
			//SnowLevel3->SetWorldScale3D(ParticleScale);
			//UE_LOG(LogTemp, Warning, TEXT("ScaleFactor: %.2f based on MaxDistance: %.1f"), ScaleFactor, MaxDistance);
		}

		if (MistParticleSystem)
		{
			MistParticleSystem->SetWorldLocation(EffectLocation);
			//MistParticleSystem->SetWorldScale3D(ParticleScale);
		}

	DrawDebugSphere(GetWorld(), EffectLocation, 50.f, 12, FColor::Green, false, 5.f);

	}

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


