#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AdaptiveWeatherSystem.h"
#include "WeatherController.generated.h"

UCLASS()
class SPM_API AWeatherController : public AActor
{
	GENERATED_BODY()

public:	
	AWeatherController();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Uppdatera när playerns performance uppdateras ig
	void UpdatePlayerPerformance(const FPerformance& PlayerPerformance);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAdaptiveWeatherSystem* WeatherSystem;

private:
	//referenser till vfx osv
	void ApplyWeatherToEnvironment();
};
