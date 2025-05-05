// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "MinigameTriggerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPM_API UMinigameTriggerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMinigameTriggerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	virtual void StartMiniGame();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* TriggerBox = nullptr;

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	virtual void ZoomIn(UPrimitiveComponent* Actor);

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	virtual void ZoomOut(UPrimitiveComponent* Actor);

	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
	APawn* MiniGamePawn = nullptr;

	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
	ACharacter* ControllerOwner = nullptr;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	AActor* MiniGameActor;

	UPROPERTY(EditAnywhere)
	bool bCompleted;

	UPROPERTY(EditAnywhere)
	bool bActive = false;
};
