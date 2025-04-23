// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "EnterLevelTriggerBox.generated.h"

class ALevelStreamingController;

/**
 * 
 */
UCLASS()
class SPM_API AEnterLevelTriggerBox : public ATriggerBox
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	AEnterLevelTriggerBox();
	
	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

private:
	UPROPERTY()
	ALevelStreamingController* LevelStreamingController;
	
	UPROPERTY(EditAnywhere)
	int RoomNumber;
	
};
