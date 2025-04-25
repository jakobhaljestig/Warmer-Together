// Fill out your copyright notice in the Description page of Project Settings.


#include "FallingTree.h"

AFallingTree::AFallingTree()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	Log = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Log"));
	RootComponent = Log;
}


void AFallingTree::BeginPlay()
{
	Super::BeginPlay();

	

}


void AFallingTree::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


