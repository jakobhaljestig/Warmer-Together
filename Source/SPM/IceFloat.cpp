// Fill out your copyright notice in the Description page of Project Settings.


#include "IceFloat.h"


// Sets default values
AIceFloat::AIceFloat()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//för mesh
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	IceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("IceMesh"));
	IceMesh->SetupAttachment(RootComponent);

	IceCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("IceCollider"));
	IceCollider->SetupAttachment(RootComponent);
	

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

//Ska göra bättre hantering av breaking sen 
void AIceFloat::BreakObject()
{
	UE_LOG(LogTemp, Warning, TEXT("BreakObject called – starting BreakTimer"));
	
	GetWorld()->GetTimerManager().SetTimer(BreakTimerHandle, this, &AIceFloat::HandleBreak, BreakTime, false);
}

void AIceFloat::HandleBreak()
{
	UE_LOG(LogTemp, Warning, TEXT("Breaking Object"));

	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
	
	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &AIceFloat::RespawnObject, RespawnTime, false);
}

void AIceFloat::RespawnObject()
{
	UE_LOG(LogTemp, Warning, TEXT("Respawning Object"));
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}

