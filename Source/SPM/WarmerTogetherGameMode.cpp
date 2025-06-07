// Copyright Epic Games, Inc. All Rights Reserved.

#include "WarmerTogetherGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AWarmerTogetherGameMode::AWarmerTogetherGameMode()
{
	DefaultPawnClass = nullptr;
}

void AWarmerTogetherGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	
}

//SKAPA BUILDER KLASS?
void AWarmerTogetherGameMode::BeginPlay()
{
	Super::BeginPlay();

}

void AWarmerTogetherGameMode::SpawnPlayers()
{
	// Spawn Player 1
	APlayerController* Player1Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (Player1Controller && Player1PawnClass)
	{
		AActor* PlayerStart = FindPlayerStart(Player1Controller, "Player1Start"); 
		FVector SpawnLocation = PlayerStart->GetActorLocation();
		FRotator SpawnRotation = PlayerStart->GetActorRotation();

		APawn* Player1Pawn = GetWorld()->SpawnActor<APawn>(Player1PawnClass, SpawnLocation, SpawnRotation);
		if (Player1Pawn)
		{
			Player1Controller->Possess(Player1Pawn);
			UE_LOG(LogTemp, Warning, TEXT("Player 1 spawned and Possessed"));
		}
	}

	// Spawn Player 2
	APlayerController* Player2Controller = UGameplayStatics::GetPlayerController(GetWorld(), 1);
	if (!Player2Controller)
	{
		Player2Controller = UGameplayStatics::CreatePlayer(GetWorld(), 1, true);
	}
	if (Player2Controller && Player2PawnClass)
	{
		AActor* PlayerStart = FindPlayerStart(Player2Controller, "Player2Start"); 
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
