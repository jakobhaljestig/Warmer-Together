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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ClimbAction;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void Crawl (const FInputActionValue& Value);
	void StopCrawl(const FInputActionValue& Value);

	void Sprint(const FInputActionValue& Value);
	void StopSprint(const FInputActionValue& Value);

	void Climb(const FInputActionValue& Value);
	
public:
	
	void Tick(float DeltaTime);

	void BeginPlay() override;

	//Tror de inte behövs UPROPERTY tbh
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Climb")
	UClimbComponent* ClimbingComponent;

};
