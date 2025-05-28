// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NiagaraComponent.h"
#include "WeatherComponent.generated.h"

enum class EWeatherLevel : uint8
{
	None,
	Light,
	Medium,
	Heavy
};

struct FPlayerSpatialInfo
{
	FVector Midpoint;
	float AvgZ;
	float MaxDistance;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPM_API UWeatherComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeatherComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/*UPROPERTY(BlueprintReadWrite)
	float TemperaturePrecent;*/

	UFUNCTION(BlueprintCallable, Category = "Weather")
	void SetWeatherEffectScale(FVector NewScale);


private:
	UPROPERTY(EditAnywhere, Category = "Weather VFX")
	class UNiagaraComponent* SnowLevel1;

	UPROPERTY(EditAnywhere, Category = "Weather VFX")
	class UNiagaraComponent* SnowLevel2;
	
	UPROPERTY(EditAnywhere, Category = "Weather VFX")
	class UNiagaraComponent* SnowLevel3;
	
	UPROPERTY(EditAnywhere, Category = "Weather VFX")
	class UNiagaraComponent* MistParticleSystem;

	UPROPERTY(EditAnywhere, Category="Niagara")
	UNiagaraSystem* SnowSystem1;

	UPROPERTY(EditAnywhere, Category="Niagara")
	UNiagaraSystem* SnowSystem2;

	UPROPERTY(EditAnywhere, Category="Niagara")
	UNiagaraSystem* SnowSystem3;

	UPROPERTY(EditAnywhere, Category="Niagara")
	UNiagaraSystem* MistSystem;

	void SpawnWeatherEffects();
	UNiagaraComponent* SpawnEffectIfNeeded(UNiagaraSystem* System, UNiagaraComponent* ExistingComp, FVector Offset) const;

	void OnWeatherUpdateTick(const TArray<AActor*>& PlayerCharacters) const;
	
	void UpdateWeatherFromTemperature(float TemperaturePercentage) const;
	
	FVector GetPlayersMidpoint() const;
	
	void UpdateWeatherEffectLocation() const;
	
	//void AffectBodyTemperatures() const;

	float CurrentCoolRate = 1.5f;

	bool bHasSpawnedWeather = false;

	static FPlayerSpatialInfo AnalyzePlayerPositions(const TArray<AActor*>& Players);
	
};
