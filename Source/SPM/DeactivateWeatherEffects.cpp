// Fill out your copyright notice in the Description page of Project Settings.


#include "DeactivateWeatherEffects.h"

#include "CharacterBase.h"
#include "Kismet/GameplayStatics.h"

ADeactivateWeatherEffects::ADeactivateWeatherEffects()
{
	OnActorBeginOverlap.AddDynamic(this, &ADeactivateWeatherEffects::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ADeactivateWeatherEffects::OnOverlapEnd);
}

void ADeactivateWeatherEffects::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (!WeatherActor)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("WeatherManager"), FoundActors);

		if (FoundActors.Num() > 0)
		{
			WeatherActor = FoundActors[0];
			UE_LOG(LogTemp, Warning, TEXT("WeatherActor hittad: %s"), *WeatherActor->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Inga WeatherManager-aktörer hittades!"));
		}
	}

	if (ACharacterBase* Character = Cast<ACharacterBase>(OtherActor))
	{
		if (WeatherActor)
		{
			if (UWeatherComponent* WeatherComp = WeatherActor->FindComponentByClass<UWeatherComponent>())
			{
				if (WeatherComp->SnowLevel1) WeatherComp->SnowLevel1->Deactivate();
				if (WeatherComp->SnowLevel2) WeatherComp->SnowLevel2->Deactivate();
				if (WeatherComp->SnowLevel3) WeatherComp->SnowLevel3->Deactivate();
				if (WeatherComp->MistParticleSystem) WeatherComp->MistParticleSystem->Deactivate();
			}
		}
	}
	
}

void ADeactivateWeatherEffects::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(OtherActor))
	{
		if (WeatherActor)
		{
			if (UWeatherComponent* WeatherComp = WeatherActor->FindComponentByClass<UWeatherComponent>())
			{
				WeatherComp->SpawnWeatherEffects();
			}
		}
	}
}
