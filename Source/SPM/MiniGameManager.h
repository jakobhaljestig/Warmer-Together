// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MiniGame.h"
#include "GameFramework/Actor.h"
#include "MiniGameManager.generated.h"

UCLASS()
class SPM_API AMiniGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMiniGameManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int CorrectPressesToWin = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int CorrectPresses = 0;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float MaxLoopTime = 10.f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float TimingOffset = 0.5f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	APawn* Game1Pawn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	APawn* Game2Pawn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UMiniGame* Game1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UMiniGame* Game2;

};
