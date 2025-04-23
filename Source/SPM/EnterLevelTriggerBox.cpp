// Fill out your copyright notice in the Description page of Project Settings.


#include "EnterLevelTriggerBox.h"

#include "CharacterBig.h"
#include "CharacterSmall.h"
#include "LevelStreamingController.h"
#include "Kismet/GameplayStatics.h"

void AEnterLevelTriggerBox::BeginPlay()
{
	Super::BeginPlay();

	LevelStreamingController = Cast<ALevelStreamingController>(
		UGameplayStatics::GetActorOfClass(this, ALevelStreamingController::StaticClass()));
}

AEnterLevelTriggerBox::AEnterLevelTriggerBox()
{
	//Register Events
	OnActorBeginOverlap.AddDynamic(this, &AEnterLevelTriggerBox::OnOverlapBegin);
}

void AEnterLevelTriggerBox::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (Cast<ACharacterBig>(OtherActor))
	{
		LevelStreamingController->SetPlayerBigLevel(RoomNumber);
	}
	if (Cast<ACharacterSmall>(OtherActor))
	{
		LevelStreamingController->SetPlayerSmallLevel(RoomNumber);
	}
}

