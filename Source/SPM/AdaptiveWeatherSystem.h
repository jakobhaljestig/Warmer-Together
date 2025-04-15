#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FPerformance.h"
#include "FWeatherState.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AdaptiveWeatherSystem.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPM_API UAdaptiveWeatherSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:	
	UAdaptiveWeatherSystem();

public:	
	// Uppdaterar systemet varje frame, inte nödvändigt om vi inte vill ha varje tick, men kan användas för periodiska uppdateringar
	virtual void Deinitialize() override;

	// Uppdaterar prestationen, t.ex. efter att någon dör eller gör något bra
	void UpdatePerformance(const FPerformance& NewPerformance);

	// Returnerar väderstatus, t.ex om det redan är dåligt väder behövs det inte adderas mer
	const FWeatherState& GetCurrentWeather() const;

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
	
	void EvaluatePerformanceAndAdjustWeather();
	
	void Tick(float DeltaTime);

	UPROPERTY(EditAnywhere, Category = "Weather")
	FWeatherState CurrentWeather;

	UPROPERTY(EditAnywhere, Category = "Performance")
	FPerformance CurrentPerformance;

	UPROPERTY(EditAnywhere, Category = "Weather")
	float UpdateInterval = 5.0f;

	float TimeSinceLastUpdate = 0.0f;
};
