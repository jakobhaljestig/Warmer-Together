// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SPMGameMode.generated.h"

class ACharacterBig;
class ACharacterSmall;

UCLASS(minimalapi)
class ASPMGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASPMGameMode();

	UFUNCTION(BlueprintCallable)
	void SpawnPlayers();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Players")
	TSubclassOf<APawn> Player1PawnClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Players")
	TSubclassOf<APawn> Player2PawnClass;
};



