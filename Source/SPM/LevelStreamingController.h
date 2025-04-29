// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelStreamingController.generated.h"

class ACharacterBig;
class ACharacterSmall;

UCLASS()
class SPM_API ALevelStreamingController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelStreamingController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetPlayerBigLevel(const int RoomNumber);

	void SetPlayerSmallLevel(const int RoomNumber);
	
protected:
	void UpdateLoadedLevels();

	void LoadLevel(const FName RoomName) const;

	void UnloadLevel(const FName RoomName) const;

	static int Difference(const int X, const int Y);

	UPROPERTY()
	TArray<FName> Rooms;
	
	UPROPERTY()
	int PlayerBigRoomNumber;

	UPROPERTY()
	int PlayerSmallRoomNumber;

};
