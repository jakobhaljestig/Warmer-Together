// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomizeComponent.h"

#include "CharacterBase.h"
#include "Rendering/RenderCommandPipes.h"

// Sets default values for this component's properties
UCustomizeComponent::UCustomizeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCustomizeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCustomizeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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

	Cast<ACharacterBase>(GetOwner())->GetMesh()->SetMaterial(0, CharacterMaterials[MaterialID]);
	
}

