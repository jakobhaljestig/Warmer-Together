// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "WarmerTogetherSaveGameSubsystem.generated.h"

class UWarmerTogetherSaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveGameSignature, class UWarmerTogetherSaveGame*, SaveObject);

/**
 * 
 */
UCLASS(meta = (DisplayName = "SaveGame System"))
class SPM_API UWarmerTogetherSaveGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:
	UPROPERTY(Transient)
	TObjectPtr<UWarmerTogetherSaveGame> CurrentSaveGame;

	

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	
};
