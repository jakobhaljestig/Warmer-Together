// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniGameManager.h"

// Sets default values
AMiniGameManager::AMiniGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMiniGameManager::BeginPlay()
{
	Super::BeginPlay();

	Game1 = Game1Pawn->GetComponentByClass<UMiniGame>();
	Game2 = Game2Pawn->GetComponentByClass<UMiniGame>();
	
	if (Game1 != nullptr && Game2 != nullptr)
	{
		Game1->SetTimer(MaxLoopTime);
		Game2->SetTimer(MaxLoopTime);
		Game1->SetInputOffset(TimingOffset);
		Game2->SetInputOffset(TimingOffset);
	}
	
}

// Called every frame
void AMiniGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Game1->IsPlayerUsing() && Game2->IsPlayerUsing())
	{
		Game1->StartGame();
		Game2->StartGame();
		if (Game1->CheckCorrectPresses() && Game2->CheckCorrectPresses())
		{
			CorrectPresses += 1;
			Game1->GameLoop();
			Game2->GameLoop();
		}

		if (CorrectPresses >= CorrectPressesToWin)
		{
			Game1->SetComplete(true);
			Game2->SetComplete(true);
		}
	}
}

