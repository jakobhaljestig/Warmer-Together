// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "InputActionValue.h"
#include "Pickup.h"
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
	UInputAction* ThrowAction;

public:
	

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	UPickup* PickupComponent;

	void BeginPlay() override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ToggleGrab(const FInputActionValue& Value);
	void Throw(const FInputActionValue& Value);

	bool IsHolding = false;
	
	
	
};
