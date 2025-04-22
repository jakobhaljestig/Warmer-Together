// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "GameFramework/Actor.h"
#include "CampFire.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UParticleSystemComponent;

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

	/*
	UFUNCTION(BlueprintCallable)
	static void OnPlayerEnterHeatZone(ACharacterBase* Player);

	UFUNCTION(BlueprintCallable)
	static void OnPlayerExitHeatZone(ACharacterBase* Player);
	*/
	
	UFUNCTION(BlueprintCallable)
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
						int32 OtherBodyIndex);

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* TriggerComp;

	UPROPERTY(EditAnywhere)
	float TriggerRadius = 500;

	bool bCheckpointActivatedCharacterBig = false;
	bool bCheckpointActivatedCharacterSmall = false;

	
};
