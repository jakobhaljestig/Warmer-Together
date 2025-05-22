// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "InputActionValue.h"
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
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* GrabAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ClimbAction;

public:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsClimbing = false;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsThrowing = false;

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	ULiftComponent* PickupComponent;

	void BeginPlay() override;

	void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ToggleGrab(const FInputActionValue& Value);

	void Climb(const FInputActionValue& Value);

	bool IsH = false;

	virtual void Move(const FInputActionValue& Value) override;

	//Tror de inte behövs UPROPERTY tbh
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Climb")
	UClimbComponent* ClimbingComponent;

private:
	
	virtual void OnDeath() override;
	
};
