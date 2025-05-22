// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdaptiveWeatherSystem.h"
#include "Components/ActorComponent.h"
#include "BodyTemperature.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPM_API UBodyTemperature : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBodyTemperature();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void IsNearHeat(bool bIsNearHeat);
	
	void ShareTemp();

	UFUNCTION(BlueprintCallable)
	float GetTempPercentage() const { return Temp / MaxTemp; }


	UFUNCTION(BlueprintCallable)
	void SetCoolDownRate(float NewRate) { CoolDownRate = NewRate; }

	void ResetTemp();

	void ModifyTemperature(float DeltaTemperature);
	void HandleFreeze();
	void HandleDeath();

	void ColdBuff(float CoolDownValue);

private:
	
	void HeatUp(float DeltaTime);

	void CoolDown(float DeltaTime);
	
	UPROPERTY(EditAnywhere, Category = "Temperature")
	float MaxTemp = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Temperature", meta = (AllowPrivateAccess = "true"))
	float CoolDownRate = 0.8f;

	UPROPERTY(EditAnywhere, Category = "Temperature")
	float HeatUpRate = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Temperature", meta = (AllowPrivateAccess = "true"))
	bool bNearHeat = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Temperature", meta = (AllowPrivateAccess = "true"))
	float Temp;

	UPROPERTY()
	UBodyTemperature* TempBigPlayer;
	UPROPERTY()
	UBodyTemperature* TempSmallPlayer;

	bool bFrozen = false;
	bool bDisplayFreezeEffect = false;

	FTimerHandle DeathTimerHandle;
	bool bHasDied = false;

	UPROPERTY(EditAnywhere)
	float DeathDelay = 1.f; // Hur lång tid efter att temperaturen nått 0 man dör
	
};

