// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelStreamingController.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ALevelStreamingController::ALevelStreamingController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Rooms = {
		FName(TEXT("RoomZero")),
		FName(TEXT("RoomOne")),
		FName(TEXT("RoomTwo")),
		FName(TEXT("RoomThree")),
		FName(TEXT("RoomFour")),
		FName(TEXT("RoomFive")),
	};
}

// Called when the game starts or when spawned
void ALevelStreamingController::BeginPlay()
{
	Super::BeginPlay();

	LoadLevel(Rooms[0]);
	LoadLevel(Rooms[1]);
}

// Called every frame
void ALevelStreamingController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelStreamingController::SetPlayerBigLevel(const int RoomNumber)
{
	PlayerBigRoomNumber = RoomNumber;
	UpdateLoadedLevels();
}

void ALevelStreamingController::SetPlayerSmallLevel(const int RoomNumber)
{
	PlayerSmallRoomNumber = RoomNumber;
	UpdateLoadedLevels();
}

void ALevelStreamingController::UpdateLoadedLevels()
{
	for (int i = 0; i < Rooms.Num(); i++)
	{
		if (Difference(i, PlayerBigRoomNumber) <= 1 || Difference(i, PlayerSmallRoomNumber) <= 1)
		{
			LoadLevel(Rooms[i]);
		}
		else
		{
			UnloadLevel(Rooms[i]);
		}
	}
}

void ALevelStreamingController::LoadLevel(const FName RoomName) const
{
	const FLatentActionInfo LatentInfo;
	UGameplayStatics::LoadStreamLevel(this, RoomName, true, false, LatentInfo);
}

void ALevelStreamingController::UnloadLevel(const FName RoomName) const
{
	const FLatentActionInfo LatentInfo;
	UGameplayStatics::UnloadStreamLevel(this, RoomName, LatentInfo, false);
}

int ALevelStreamingController::Difference(const int X, const int Y)
{
	return FMath::Abs(X - Y);
}

