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
	UFUNCTION(BlueprintImplementableEvent, Category="ShiveringEffects")
	void StartShiveringEffects(float TempPercentage);

	UFUNCTION(BlueprintImplementableEvent, Category="ShiveringEffects")
	void StopShiveringEffects();

	UFUNCTION(BlueprintImplementableEvent, Category="ShiveringEffects")
	void ColdBuffFeedBack();
	
	UFUNCTION(BlueprintCallable)
	void HideHUD(bool bShouldHide);

	UFUNCTION(BlueprintCallable)
	void SetCanShiver(bool bShouldBeAbleToShiver) { bCanShiver = bShouldBeAbleToShiver; }

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
	
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess = true))
	bool bCanShiver = true;
	
};
