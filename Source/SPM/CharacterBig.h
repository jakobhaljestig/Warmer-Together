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
	UInputAction* ThrowAction;

	//TA BORT
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ClimbAction;

public:
	

protected:

	//TA BORT
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Climb")
	UClimbComponent* ClimbingComponent;

	void Climb(const FInputActionValue& Value);

	//Till character small 
	virtual void Move(const FInputActionValue& Value) override;
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	ULiftComponent* PickupComponent;

	void BeginPlay() override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ToggleGrab(const FInputActionValue& Value);
	void Throw(const FInputActionValue& Value);

	bool IsH = false;

	//Ska till small 
	bool bIsClimbing = false;
	
	
	
};
