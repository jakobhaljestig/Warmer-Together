// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "ClimbComponent.h"
#include "CharacterSmall.generated.h"

UCLASS()
class SPM_API ACharacterSmall : public ACharacterBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrawlAction;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void Crawl (const FInputActionValue& Value);
	void StopCrawl(const FInputActionValue& Value);

	virtual void BeginPush(const FInputActionValue& Value);
	
public:
	void Tick(float DeltaTime);

	void BeginPlay() override;

	void ResetAnimations() override;

private:

	void OnDeath() override;

	bool bIsCrawling = false;
	
};
