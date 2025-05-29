// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SprintComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPM_API USprintComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USprintComponent();
	
	void StartSprint ();
	
	void StopSprint();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	float SprintSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	float MaxStamina = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	float StaminaDrainRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	float StaminaRegenRate;

	UFUNCTION(BlueprintCallable, Category = "Sprint")
	float GetStamina() {return Stamina;}
	

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	
	void DrainStamina();
	
	void RegenerateStamina();

	bool bCanSprint = true;
	
	bool bIsRegenerating;

	float Stamina;

	FTimerHandle StaminaCooldownTimerHandle;
};
