// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "UnloadLevelTriggerBox.generated.h"

/**
 * 
 */
UCLASS()
class SPM_API AUnloadLevelTriggerBox : public ATriggerBox
{
	GENERATED_BODY()
	
public:
	AUnloadLevelTriggerBox();
	
	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

private:	
	UPROPERTY(EditAnywhere)
	FName LevelToUnload;

	UPROPERTY()
	bool P1Passed;

	UPROPERTY()
	bool P2Passed;
	
};
