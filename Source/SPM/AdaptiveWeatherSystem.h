#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FPerformance.h"
#include "FWeatherState.h"
#include "AdaptiveWeatherSystem.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPM_API UAdaptiveWeatherSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAdaptiveWeatherSystem();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Uppdaterar prestationen, t.ex. efter att någon dör eller gör något bra?
	void RegisterPerformanceEvent(const FPerformance& NewPerformance);

	// Returnerar väderstatus, t.ex om det redan är dåligt väder behövs det inte adderas mer
	const FWeatherState& GetCurrentWeather() const;

private:
	
	void EvaluatePerformanceAndAdjustWeather();

	UPROPERTY(EditAnywhere, Category = "Weather")
	FWeatherState CurrentWeather;

	UPROPERTY(EditAnywhere, Category = "Performance")
	FPerformance CurrentPerformance;

	UPROPERTY(EditAnywhere, Category = "Weather")
	float UpdateInterval = 5.0f;

	float TimeSinceLastUpdate = 0.0f;
};
