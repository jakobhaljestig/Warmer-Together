// Fill out your copyright notice in the Description page of Project Settings.


#include "MusicTrigger.h"

// Sets default values
AMusicTrigger::AMusicTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMusicTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMusicTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

