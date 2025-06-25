// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterPlayerController.h"
#include "MiniGame.h"
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

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
						int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* TriggerBox = nullptr;
	

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	APawn* MiniGamePawn = nullptr;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	ACharacter* ControllerOwner = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TSubclassOf<UUserWidget> MiniGameWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TSubclassOf<UUserWidget> VictoryWidget;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	AActor* MiniGameActor = nullptr;
	
	UPROPERTY(EditAnywhere)
	ACharacterPlayerController* Controller = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCompleted = false;

	UPROPERTY(EditAnywhere)
	bool bActive = false;

	UFUNCTION(BlueprintCallable)
	bool GetCompletion() const { return bCompleted; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMiniGame* MiniGame = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ForBigPlayer = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ForSmallPlayer = false;

	UFUNCTION(BlueprintCallable)
	virtual void ZoomIn(AActor* Actor);

	UFUNCTION(BlueprintCallable)
	virtual void ZoomOut();

	UFUNCTION(BlueprintCallable)
	virtual void Start();
};
