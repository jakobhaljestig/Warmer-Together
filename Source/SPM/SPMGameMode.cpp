// Copyright Epic Games, Inc. All Rights Reserved.

#include "SPMGameMode.h"
#include "CharacterBig.h"
#include "CharacterSmall.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "CharacterPlayerController.h"

ASPMGameMode::ASPMGameMode()
{
	DefaultPawnClass = nullptr;
}

void ASPMGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// (Save/Load logic moved into new SaveGameSubsystem)
	//URogueSaveGameSubsystem* SG = GetGameInstance()->GetSubsystem<URogueSaveGameSubsystem>();

	// Optional slot name (Falls back to slot specified in SaveGameSettings class/INI otherwise)
	//FString SelectedSaveSlot = UGameplayStatics::ParseOption(Options, "SaveGame");
	//SG->LoadSaveGame(SelectedSaveSlot);
}

//SKAPA BUILDER KLASS NORA??
void ASPMGameMode::BeginPlay()
{
	//Spawnar Spelare 1
	APlayerController* Player1Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (Player1Controller && Player1PawnClass)
	{
		AActor* PlayerStart = FindPlayerStart(Player1Controller, "Player1Start"); //lägg till tag för spawning point player 1
		FVector SpawnLocation = PlayerStart->GetActorLocation();
		FRotator SpawnRotation = PlayerStart->GetActorRotation();

		APawn* Player1Pawn = GetWorld()->SpawnActor<APawn>(Player1PawnClass, SpawnLocation, SpawnRotation);
		if (Player1Pawn)
		{
			Player1Controller->Possess(Player1Pawn);
			UE_LOG(LogTemp, Warning, TEXT("Player 1 spawned and Possessed"));
		}
	}

	//Spawna player 2
	
	APlayerController* Player2Controller = UGameplayStatics::CreatePlayer(GetWorld(), 1, true);
	if (Player2Controller && Player2PawnClass)
	{
		AActor* PlayerStart = FindPlayerStart(Player2Controller, "Player2Start"); //lägg till tag för spawning point player 1
		FVector SpawnLocation = PlayerStart->GetActorLocation();
		FRotator SpawnRotation = PlayerStart->GetActorRotation();

		APawn* Player2Pawn = GetWorld()->SpawnActor<APawn>(Player2PawnClass, SpawnLocation, SpawnRotation);
		if (Player2Pawn)
		{
			Player2Controller->Possess(Player2Pawn);
			UE_LOG(LogTemp, Warning, TEXT("Player 2 spawned and Possessed"));
		}
	}
}
