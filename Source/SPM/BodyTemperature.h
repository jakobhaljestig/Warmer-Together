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

	void CoolDown(float DeltaTime);
	void HeatUp(float DeltaTime);
	void ShareTemp();

	UFUNCTION(BlueprintCallable)
	float GetTemp() const { return Temp; }

private:	
	UPROPERTY(EditAnywhere, Category = "Temperature")
	float MaxTemp = 100.f;

	UPROPERTY(EditAnywhere, Category = "Temperature")
	float CoolDownRate = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Temperature")
	float HeatUpRate = 2.f;

	UPROPERTY(EditAnywhere, Category = "Temperature")
	bool bShouldCoolDown = false;

	UPROPERTY(EditAnywhere, Category = "Temperature")
	bool bShouldHeatUp = false;

	UPROPERTY(EditAnywhere, Category = "Temperature")
	bool bHugging = false;

	UPROPERTY(EditAnywhere, Category = "Temperature")
	float Temp;

	UBodyTemperature* TempBig = nullptr;
	UBodyTemperature* TempSmall = nullptr;
};

