// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "CharacterBig.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class SPM_API ACharacterBig : public ACharacterBase
{
	GENERATED_BODY()



public:
	void Move(const FInputActionValue& Value) override;

protected:
	
	
};
