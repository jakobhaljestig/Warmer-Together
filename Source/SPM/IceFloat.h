// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IceFloat.generated.h"

UCLASS()
class SPM_API AIceFloat : public AActor
{
	GENERATED_BODY()
	
public:	
	AIceFloat();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "IceFloat")
	void BreakObject();

	UFUNCTION(BlueprintCallable, Category = "IceFloat")
	void RepawnObject();

private:

	void CountDown(float Timer); 
	
	FTimerHandle CountDownTimer;
	
	float RespawnTime = 5.0f;
	float BreakTime = 2.0f;
	

};
