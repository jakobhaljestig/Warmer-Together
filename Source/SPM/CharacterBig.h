// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "ClimbComponent.h"
#include "LiftComponent.h"
#include "CharacterBig.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class SPM_API ACharacterBig : public ACharacterBase
{
	GENERATED_BODY()

	ACharacterBig();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* GrabAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ClimbAction;

public:

	virtual bool IsClimbing() const override { return bIsClimbing; }
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsClimbing = false;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsThrowing = false;

	void ResetPlayerState() override;

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
	ULiftComponent* LiftComponent;

	void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ToggleGrab(const FInputActionValue& Value);

	void BeginHug(const FInputActionValue& Value) override;
	
	void Climb(const FInputActionValue& Value);

	bool IsH = false;

	virtual void Move(const FInputActionValue& Value) override;

	//Tror de inte behövs UPROPERTY tbh
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Climb")
	UClimbComponent* ClimbComponent;

private:
	
	virtual void OnDeath() override;
	
};
