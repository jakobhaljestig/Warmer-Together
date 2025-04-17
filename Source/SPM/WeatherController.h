#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AdaptiveWeatherSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "WeatherController.generated.h"

UCLASS()
class SPM_API AWeatherController : public AActor
{
	GENERATED_BODY()

public:	
	// Konstruktor
	AWeatherController();
	
protected:
	// Kallas när spelet startar eller när aktören spawnas
	virtual void BeginPlay() override;

public:	
	// Kallas varje frame
	virtual void Tick(float DeltaTime) override;

	// Uppdatera spelarens prestation
	UFUNCTION(BlueprintCallable, Category = "Weather")
	void UpdatePlayerPerformance(const FPerformance& PlayerPerformance);

	// Väder-systemet som referens
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weather")
	UAdaptiveWeatherSystem* WeatherSystem;

	UFUNCTION(BlueprintCallable)
	void SimulateBadPerformance();

	UFUNCTION(BlueprintCallable)
	void SimulateGoodPerformance();

	UPROPERTY(EditAnywhere, Category = "Weather VFX")
	class AExponentialHeightFog* FogActor;

	UPROPERTY(EditAnywhere, Category = "Weather VFX")
	class UNiagaraComponent* SnowParticleSystem;


private:
	// Funktion för att applicera väderförändringar på miljön
	void ApplyWeatherToEnvironment() const;
};
