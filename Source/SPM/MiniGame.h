// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MiniGame.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPM_API UMiniGame : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMiniGame();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	bool bIsComplete = false;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float GameSpeed = 50.f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float TimingOffset = 3.f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float TimeSinceLastPress = 0;

	UPROPERTY()
	int LastInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int RequestedInput = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int CorrectPressesToWin = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int CorrectPresses = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float CurrentLoopTime = 0;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float MaxLoopTime = 10.f;
	
	UPROPERTY()
	bool bUsing = false;
	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	bool IsComplete() const { return bIsComplete; }
	
	UFUNCTION(BlueprintCallable)
	void GameLoop();

	UFUNCTION(BlueprintCallable)
	void ReadInput(int Input);

	UFUNCTION(BlueprintCallable)
	int DecideInput();

	UFUNCTION(BlueprintCallable)
	bool CheckCorrectPresses();

	UFUNCTION(BlueprintCallable)
	FText ShownInput();

	UFUNCTION(BlueprintCallable)
	void SetTimer(const float LoopTime){ MaxLoopTime = LoopTime; }
	
	UFUNCTION(BlueprintCallable)
	void SetInputOffset(const float Offset){ TimingOffset = Offset; }
	
	UFUNCTION(BlueprintCallable)
	void SetComplete(const bool CompletionStatus){ bIsComplete = CompletionStatus; }

	UFUNCTION(BlueprintCallable)
	void StartGame(){ bPlaying = true; }

	UFUNCTION()
	bool IsPlayerUsing() const { return bUsing; }

	UFUNCTION(BlueprintCallable)
	void SetPlayerUsing(bool Using) { bUsing = Using; }

	UFUNCTION(BlueprintCallable)
	float GetTimer() const { return CurrentLoopTime; }

	UFUNCTION(BlueprintCallable)
	float GetMaxTime() const { return MaxLoopTime; }

	UPROPERTY()
	bool Correct = false;

	UPROPERTY()
	bool bPlaying = false;
};
