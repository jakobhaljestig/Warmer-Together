#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FPerformance.h"
#include "FWeatherState.h"
#include "WeatherUpdaterInterface.h"
#include "Engine/ExponentialHeightFog.h"
#include "NiagaraComponent.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AdaptiveWeatherSystem.generated.h"

UENUM(BlueprintType)
enum class EZoneType : uint8
{
	ZONE_NEUTRAL UMETA(DisplayName = "Neutral"),
	ZONE_MEDIUM UMETA(DisplayName = "Medium"),
	ZONE_INTENSE UMETA(DisplayName = "Intense")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPM_API UAdaptiveWeatherSystem : public UGameInstanceSubsystem,  public IWeatherUpdaterInterface
{
	GENERATED_BODY()

public:	
	UAdaptiveWeatherSystem();
	
	virtual void BeginPlay();

	EZoneType GetCurrentZone() const { return CurrentZone; }
	// Uppdaterar systemet varje frame, inte nödvändigt om vi inte vill ha varje tick, men kan användas för periodiska uppdateringar
	virtual void Deinitialize() override;
	void OnMapLoaded(UWorld* LoadedWorld);
	void OnWeatherUpdateTick();
	

	// Returnerar väderstatus, t.ex om det redan är dåligt väder behövs det inte adderas mer
	const FWeatherState& GetCurrentWeather() const;
	void InitializeEnvironmentReferences();
	void UpdateWeatherFromTemperature(float TemperaturePercentage) const;
	FVector GetPlayersMidpoint() const;
	
	void UpdateWeatherEffectLocation() const;
	//void OnWeatherUpdateTick() const;

	// Uppdaterar prestationen, t.ex. efter att någon dör eller gör något bra
	virtual void UpdatePerformance(const FPerformance& NewPerformance) override;

	//void SetCurrentZone(EZoneType NewZone);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather")
	AExponentialHeightFog* FogActor;

	UPROPERTY(EditAnywhere, Category = "Weather VFX")
	class UNiagaraComponent* SnowLevel3;

	UPROPERTY(EditAnywhere, Category = "Weather VFX")
	class UNiagaraComponent* SnowLevel2;

	UPROPERTY(EditAnywhere, Category = "Weather VFX")
	class UNiagaraComponent* SnowLevel1;
	
	UPROPERTY(EditAnywhere, Category = "Weather VFX")
	class UNiagaraComponent* MistParticleSystem;

	/*
	void ApplyEnvironmentEffects() const;
	void AggregatePerformance();
	*/

	mutable float CachedEnvTemp = 0.0f;

	bool bIsCooperationDetected = false;

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:

	EZoneType CurrentZone = EZoneType::ZONE_NEUTRAL; // Startzon

	void AffectBodyTemperatures() const;
	
	//void EvaluatePerformanceAndAdjustWeather();
	
	UPROPERTY(EditAnywhere, Category = "Weather")
	FWeatherState CurrentWeather;

	UPROPERTY(EditAnywhere, Category = "Performance")
	FPerformance CurrentPerformance;

	UPROPERTY(EditAnywhere, Category = "Weather")
	float UpdateInterval = 5.0f;

	float TimeSinceLastUpdate = 0.0f;

	float CurrentCoolRate = 1.5f;

	int32 WeatherLevel = GetCurrentWeather().WeatherLevel;

	FTimerHandle WeatherUpdateTimerHandle;


};
