// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IceFloe.generated.h"

UCLASS()
class SPM_API AIceFloe : public AActor
{
	GENERATED_BODY()
	
public:	
	AIceFloe();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle PlatformTimer;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "IceFloe")
	void BreakIceFloe();

	UFUNCTION(BlueprintCallable, Category = "IceFloe")
	void respawnIceFloe();

private:

	void StartFallTimer();
	void StartRespawnTimer();
	
	float BreakTimer = 2;
	float respawnTimer = 5;
	

};
