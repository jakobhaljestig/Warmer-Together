// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MusicManager.h"
#include "GameFramework/Actor.h"
#include "MusicTrigger.generated.h"

UCLASS()
class SPM_API AMusicTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMusicTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music")
	USoundWave* Track;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music")
	AMusicManager* MusicManager;

	void SetTrack(USoundWave* NewTrack) { Track = NewTrack; };

};
