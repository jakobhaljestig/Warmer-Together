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
	static void ShareTemp();

private:
	UPROPERTY(EditAnywhere)
	UBodyTemperature* OtherPlayersBodyTemperature;
	
	UPROPERTY(EditAnywhere)
	float MaxTemp = 100.f;

	UPROPERTY(EditAnywhere)
	float CoolDownRate = 0.5f;

	UPROPERTY(EditAnywhere)
	float HeatUpRate = 2.f;

	UPROPERTY(EditAnywhere)
	float ShareTempRate = 0.1f;

	UPROPERTY(EditAnywhere)
	bool bShouldCoolDown = false;

	UPROPERTY(EditAnywhere)
	bool bHugging = false;
	
	float Temp;

	
	
};

