// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "GameFramework/Actor.h"
#include "CampFire.generated.h"

UCLASS()
class SPM_API ACampFire : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACampFire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Referens till vår performance-tracker
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Performance")
	UPerformanceTracker* PerformanceTracker;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	static void OnPlayerEnterHeatZone(ACharacterBase* Player);
	static void OnPlayerExitHeatZone(ACharacterBase* Player);


private:
	UPROPERTY(EditAnywhere, Category = "Fire")
	float Radius = 5.f;

};
