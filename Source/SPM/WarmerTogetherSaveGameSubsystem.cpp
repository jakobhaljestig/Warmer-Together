// Fill out your copyright notice in the Description page of Project Settings.


#include "WarmerTogetherSaveGameSubsystem.h"

#include "EngineUtils.h"
#include "SavableObjectInterface.h"
#include "WarmerTogetherSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

void UWarmerTogetherSaveGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	CurrentSlotName = "WarmerTogetherSaveGame";
}

void UWarmerTogetherSaveGameSubsystem::WriteSaveGame()
{
	// ... < playerstate saving code ommitted >

	// Clear all actors from any previously loaded save to avoid duplicates
	CurrentSaveGame->SavedActors.Empty();

	// Iterate the entire world of actors
	for (AActor* Actor : TActorRange<AActor>(GetWorld()))
	{
		// Only interested in savable objects, skip actors that are being destroyed
		if (!IsValid(Actor) || !Actor->Implements<USavableObjectInterface>())
		{
			continue;
		}

		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetFName();
		ActorData.Transform = Actor->GetActorTransform();
	
		// Pass the array to fill with data from Actor
		FMemoryWriter MemWriter(ActorData.ByteData);

		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
		// Find only variables with UPROPERTY(SaveGame)
		Ar.ArIsSaveGame = true;
		// Converts Actor's SaveGame UPROPERTIES into binary array
		Actor->Serialize(Ar);

		CurrentSaveGame->SavedActors.Add(Actor->GetFName(), ActorData);
	}

	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, CurrentSlotName, 0);

	OnSaveGameWritten.Broadcast(CurrentSaveGame);
}

void UWarmerTogetherSaveGameSubsystem::LoadSaveGame(FString InSlotName)
{
	CurrentSlotName = InSlotName;

	if (UGameplayStatics::DoesSaveGameExist(CurrentSlotName, 0))
	{
		CurrentSaveGame = Cast<UWarmerTogetherSaveGame>(UGameplayStatics::LoadGameFromSlot(CurrentSlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			return;
		}

		// Iterate the entire world of actors
		for (AActor* Actor : TActorRange<AActor>(GetWorld()))
		{
			// Only interested in savable objects
			if (!Actor->Implements<USavableObjectInterface>())
			{
				continue;
			}

			if (FActorSaveData* FoundData = CurrentSaveGame->SavedActors.Find(Actor->GetFName()))
			{
				Actor->SetActorTransform(FoundData->Transform);

				FMemoryReader MemReader(FoundData->ByteData);

				FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
				Ar.ArIsSaveGame = true;
				// Convert binary array back into actor's variables
				Actor->Serialize(Ar);

				ISavableObjectInterface::Execute_OnActorLoaded(Actor);
			}
		}

		OnSaveGameLoaded.Broadcast(CurrentSaveGame);
	}
	else
	{
		CurrentSaveGame = CastChecked<UWarmerTogetherSaveGame>(UGameplayStatics::CreateSaveGameObject(UWarmerTogetherSaveGame::StaticClass()));
	}
}
