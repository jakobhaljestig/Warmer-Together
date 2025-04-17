// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SavableObjectInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USavableObjectInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SPM_API ISavableObjectInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void OnActorLoaded();
public:
};
