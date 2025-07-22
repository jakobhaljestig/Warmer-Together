// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniGameManager.h"

#include "MinigameTriggerComponent.h"

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
		Game1->MiniGameManager = this;
		Game2->MiniGameManager = this;
	}
	
}

// Called every frame
void AMiniGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Game1->IsPlayerUsing() && Game2->IsPlayerUsing())
	{
		if (!Game1->bPlaying && !Game2->bPlaying)
		{
			Game1->StartGame();
			Game2->StartGame();
		}
	}
}

void AMiniGameManager::CheckPresses()
{
	if (CorrectPresses >= CorrectPressesToWin)
	{
		Game1->SetComplete(true);
		Game2->SetComplete(true);
		Game1Pawn->GetComponentByClass<UMinigameTriggerComponent>()->ZoomOut();
		Game2Pawn->GetComponentByClass<UMinigameTriggerComponent>()->ZoomOut();
			
	}

	if (Game1->Correct && Game2->Correct)
	{
		CorrectPresses += 1;
		Game1->GameLoop();
		Game2->GameLoop();
	}
}

