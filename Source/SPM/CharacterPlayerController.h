// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CharacterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SPM_API ACharacterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void HideHUD(bool bShouldHide);

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> FreezeEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* FreezeEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* HUD;
	
protected:
	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;
	
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> HUDClass;
	
	

	
	
};
