// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniGame.h"

// Sets default values for this component's properties
UMiniGame::UMiniGame()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMiniGame::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMiniGame::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CurrentLoopTime += DeltaTime;
	if (CurrentLoopTime > MaxLoopTime)
	{
		GameLoop();
	}

	if (CorrectPressesToWin == CorrectPresses)
	{
		bIsComplete = true;
	}
	// ...
}

void UMiniGame::GameLoop()
{
	RequestedInput = DecideInput();
	LastInput = 0;
	CurrentLoopTime = 0;
}

void UMiniGame::ReadInput(const int Input)
{
	LastInput = Input;
}

int UMiniGame::DecideInput()
{
	return FMath::RandRange(1, 4);
}

bool UMiniGame::CheckCorrectPresses()
{
	if (RequestedInput == LastInput)
	{
		CorrectPresses += 1;
		return true;	
	}
	return false;
}

FText UMiniGame::ShownInput()
{
	if (RequestedInput == 1)
	{
		return FText::FromString("Y");
	}
	if (RequestedInput == 2)
	{
		return FText::FromString("B");
	}
	if (RequestedInput == 3)
	{
		return FText::FromString("X");
	}
	if (RequestedInput == 4)
	{
		return FText::FromString("A");
	}
	return FText::FromString("");
}

