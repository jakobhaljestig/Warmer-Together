// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Health.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPM_API UHealth : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealth();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	float GetHealthPercentage() const { return Health / MaxHealth; }

	void UpdateHealthOnFrozen(float DeltaTime);

	void IsFrozen(bool bShouldBeFrozen);

private:
	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth = 100;

	UPROPERTY(EditAnywhere, Category = "Health")
	float HealthDownRate = 5.f;
	
	float Health;
	bool bFrozen = false;

	
};
