// Fill out your copyright notice in the Description page of Project Settings.

#include "CampFire.h"
#include "CharacterBase.h"
#include "PerformanceTracker.h"


// Sets default values
ACampFire::ACampFire()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACampFire::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACampFire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Exempel på trigger när spelaren går in/ut ur en värmezons område
void ACampFire::OnPlayerEnterHeatZone(ACharacterBase* Player)
{
	// När spelaren går in i värmezonen
	Player->PerformanceTracker->SetIsNearHeat(true);
}

void ACampFire::OnPlayerExitHeatZone(ACharacterBase* Player)
{
	// När spelaren går ut ur värmezonen
	Player->PerformanceTracker->SetIsNearHeat(false);
}


