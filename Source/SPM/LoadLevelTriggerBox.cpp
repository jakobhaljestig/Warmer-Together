// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadLevelTriggerBox.h"

#include "CharacterBase.h"
#include "Kismet/GameplayStatics.h"

ALoadLevelTriggerBox::ALoadLevelTriggerBox()
{
	OnActorBeginOverlap.AddDynamic(this, &ALoadLevelTriggerBox::OnOverlapBegin);
}

void ALoadLevelTriggerBox::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (Cast<ACharacterBase>(OtherActor) && LevelToLoad != "")
	{
		UE_LOG(LogTemp, Warning, TEXT("Loading %s"), *LevelToLoad.ToString());
		const FLatentActionInfo LatentInfo;
		UGameplayStatics::LoadStreamLevel(this, LevelToLoad, true, false, LatentInfo);

		Destroy();
	}
}
