// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomizeComponent.h"

// Sets default values for this component's properties
UCustomizeComponent::UCustomizeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}


// Called when the game starts
void UCustomizeComponent::BeginPlay()
{
	Super::BeginPlay();
	SetMaterialByIndex(MaterialID);
	// ...
	
}
void UCustomizeComponent::NextMaterial()
{
	if (MaterialID == CharacterMaterials.Num() - 1)
	{
		MaterialID = 0;
	}
	else
	{
		MaterialID++;
	}

	GetOwner()->GetComponentByClass<USkeletalMeshComponent>()->SetMaterial(0, CharacterMaterials[MaterialID]);
	
}

void UCustomizeComponent::PreviousMaterial()
{
	if (MaterialID == -1)
	{
		MaterialID = CharacterMaterials.Num() - 1;
	}
	else
	{
		MaterialID--;
	}

	GetOwner()->GetComponentByClass<USkeletalMeshComponent>()->SetMaterial(0, CharacterMaterials[MaterialID]);
}

void UCustomizeComponent::SetMaterialByIndex(const int MaterialIndex)
{
	if (MaterialIndex > CharacterMaterials.Num() - 1|| MaterialIndex <= -1)
	{
		return;
	}
	GetOwner()->GetComponentByClass<USkeletalMeshComponent>()->SetMaterial(0, CharacterMaterials[MaterialID]);
	MaterialID = MaterialIndex;
}

