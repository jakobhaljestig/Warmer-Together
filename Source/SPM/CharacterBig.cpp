// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBig.h"


void ACharacterBig::Move(const FInputActionValue& Value)
{
	Super::Move(Value);

	UE_LOG(LogTemp, Warning, TEXT("CharacterBig i Moving"));
}

