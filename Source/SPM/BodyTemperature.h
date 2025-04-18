// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

private:
	void CoolDown(float DeltaTime);
	void HeatUp(float DeltaTime);
	
	UPROPERTY(EditAnywhere, Category = "Temperature")
	float MaxTemp = 100.f;

	UPROPERTY(EditAnywhere, Category = "Temperature")
	float CoolDownRate = 2.5f;

	UPROPERTY(EditAnywhere, Category = "Temperature")
	float HeatUpRate = 20.f;

	UPROPERTY(EditAnywhere, Category = "Temperature")
	bool bNearHeat = false;

	UPROPERTY(EditAnywhere, Category = "Temperature")
	float Temp;

	UPROPERTY()
	UBodyTemperature* TempBigPlayer;
	UPROPERTY()
	UBodyTemperature* TempSmallPlayer;

	bool bFrozen = false;
	bool bDisplayFreezeEffect = false;
};

