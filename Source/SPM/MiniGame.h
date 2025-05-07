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
	bool bIsComplete;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float GameSpeed;

	UPROPERTY(EditAnywhere)
	float AllowedTimingDifference;

	UPROPERTY()
	int LastInput;
	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	bool IsComplete() const { return bIsComplete; }

	UFUNCTION()
	void GameLoop();

	UFUNCTION(BlueprintCallable)
	void ReadInput(int Input);

	UFUNCTION(BlueprintCallable)
	int DecideInput();
};
