// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterBase.h"
#include "SprintComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPM_API USprintComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USprintComponent();
	
	void StartSprint (const FInputActionValue& Value);
	
	void StopSprint(const FInputActionValue& Value);

protected:
	virtual void BeginPlay() override;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	float SprintSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	float MaxStamina = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	float StaminaDrainRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	float StaminaRegenRate = 2;
	

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	
	void DrainStamina();
	
	void RegenerateStamina();

	bool bCanSprint;
	
	bool bIsRegenerating;

	float Stamina;

	FTimerHandle StaminaCooldownTimerHandle;
};
