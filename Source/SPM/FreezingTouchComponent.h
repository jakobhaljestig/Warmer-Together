// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdaptiveWeatherSystem.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "FreezingTouchComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPM_API UFreezingTouchComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFreezingTouchComponent();

	UPROPERTY(EditAnywhere)
	float CoolDownRate;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	UBoxComponent* TriggerBox;

	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool bHasDied = false;
		
};
