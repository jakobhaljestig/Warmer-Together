// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "LoadLevelTriggerBox.generated.h"

/**
 * 
 */
UCLASS()
class SPM_API ALoadLevelTriggerBox : public ATriggerBox
{
	GENERATED_BODY()

public:
	ALoadLevelTriggerBox();
	
	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

private:	
	UPROPERTY(EditAnywhere)
	FName LevelToLoad;
	
};
