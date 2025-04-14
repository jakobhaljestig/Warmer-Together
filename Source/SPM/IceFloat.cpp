// Fill out your copyright notice in the Description page of Project Settings.


#include "IceFloat.h"

// Sets default values
AIceFloat::AIceFloat()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AIceFloat::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AIceFloat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AIceFloat::RepawnObject()
{
	
}

void AIceFloat::BreakObject()
{
	
}

