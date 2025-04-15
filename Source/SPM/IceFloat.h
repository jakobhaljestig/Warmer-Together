// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CountdownTimer.h"
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


private:

	CountdownTimer Timer;

	void RespawnObject();
	
	float RespawnTime = 2.5f;
	float BreakTime = 2.0f;
	

};
