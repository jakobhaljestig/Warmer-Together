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
	if (!bHasRespawn) return;
	
	UE_LOG(LogTemp, Warning, TEXT("BreakObject called – starting BreakTimer"));
	bHasRespawn = false;

	BreakTime = BreakTime - 0.5f;

	GetWorld()->GetTimerManager().SetTimer(IcefloatTimerHandle, this, &AIceFloat::TriggerAnimationBreak, BreakTime, false);
}

void AIceFloat::TriggerAnimationBreak()
{
	UE_LOG(LogTemp, Warning, TEXT("TriggerAnimationBreak"));

	if (BreakTime > 0.0f)
	{
		BreakEvent();
	}
	
	GetWorld()->GetTimerManager().SetTimer(IcefloatTimerHandle, this, &AIceFloat::HandleBreak, 0.5f, false);
}

void AIceFloat::HandleBreak()
{
	UE_LOG(LogTemp, Warning, TEXT("Breaking Object"));

	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
	
	GetWorld()->GetTimerManager().SetTimer(IcefloatTimerHandle, this, &AIceFloat::RespawnAnimation, RespawnTime, false);
}

void AIceFloat::RespawnAnimation()
{
	UE_LOG(LogTemp, Warning, TEXT("RespawnAnimation Triggered"));
	SetActorHiddenInGame(false);
	RespawnEvent();
}


void AIceFloat::RespawnObject()
{
	UE_LOG(LogTemp, Warning, TEXT("Respawning Object"));
	SetActorEnableCollision(true);
	bHasRespawn = true;
}




