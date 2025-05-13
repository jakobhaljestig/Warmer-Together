// Fill out your copyright notice in the Description page of Project Settings.


#include "UnloadLevelTriggerBox.h"

#include "CharacterBig.h"
#include "CharacterSmall.h"
#include "Kismet/GameplayStatics.h"

AUnloadLevelTriggerBox::AUnloadLevelTriggerBox()
{
	OnActorBeginOverlap.AddDynamic(this, &AUnloadLevelTriggerBox::OnOverlapBegin);
}

void AUnloadLevelTriggerBox::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (Cast<ACharacterBig>(OtherActor))
	{
		P1Passed = true;
	}
	if (Cast<ACharacterSmall>(OtherActor))
	{
		P2Passed = true;
	}

	if (P1Passed && P2Passed)
	{
		UE_LOG(LogTemp, Warning, TEXT("Unloading %s"), *LevelToUnload.ToString());
		const FLatentActionInfo LatentInfo;
		UGameplayStatics::UnloadStreamLevel(this, LevelToUnload, LatentInfo, true);
	}
}
