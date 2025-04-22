// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelStreamingController.h"

#include "Engine/LevelScriptActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALevelStreamingController::ALevelStreamingController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALevelStreamingController::BeginPlay()
{
	Super::BeginPlay();

	Rooms = {
		FName(TEXT("Room0")),
		FName(TEXT("Room1")),
		FName(TEXT("Room2")),
		FName(TEXT("Room3")),
		FName(TEXT("Room4")),
	};
	FLatentActionInfo LatentInfo;
	UGameplayStatics::LoadStreamLevel(this, Rooms[0], true, false, LatentInfo);
}

// Called every frame
void ALevelStreamingController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelStreamingController::SetPlayerBigLevel(const int RoomNumber)
{
	PlayerBigRoomNumber = RoomNumber;
}

void ALevelStreamingController::SetPlayerSmallLevel(const int RoomNumber)
{
	PlayerSmallRoomNumber = RoomNumber;
}

void ALevelStreamingController::UpdateLoadedLevels()
{
	FLatentActionInfo LatentInfo;

	if (PlayerBigRoomNumber == PlayerSmallRoomNumber)
	{
		for (int i = 0; i < Rooms.Num(); i++)
		{
			if (i != PlayerBigRoomNumber - 1 || i != PlayerBigRoomNumber || i != PlayerBigRoomNumber + 1)
			{
				UGameplayStatics::UnloadStreamLevel(this, Rooms[i], LatentInfo, false);
			}
			else
			{
				UGameplayStatics::LoadStreamLevel(this, Rooms[i], true, false, LatentInfo);
			}
		}
	}
}

