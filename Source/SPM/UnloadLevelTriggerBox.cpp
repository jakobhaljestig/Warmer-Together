// Fill out your copyright notice in the Description page of Project Settings.


#include "UnloadLevelTriggerBox.h"

#include "CharacterBase.h"
#include "Kismet/GameplayStatics.h"

AUnloadLevelTriggerBox::AUnloadLevelTriggerBox()
{
	OnActorBeginOverlap.AddDynamic(this, &AUnloadLevelTriggerBox::OnOverlapBegin);
}

void AUnloadLevelTriggerBox::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (Cast<ACharacterBase>(OtherActor) && LevelToUnload != "")
	{
		UE_LOG(LogTemp, Warning, TEXT("Unloading %s"), *LevelToUnload.ToString());
		const FLatentActionInfo LatentInfo;
		UGameplayStatics::UnloadStreamLevel(this, LevelToUnload, LatentInfo, false);
	}
}
