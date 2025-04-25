// Fill out your copyright notice in the Description page of Project Settings.


#include "FallingTree.h"


AFallingTree::AFallingTree()
{
 	
	PrimaryActorTick.bCanEverTick = true;

}


void AFallingTree::BeginPlay()
{
	Super::BeginPlay();
	StaticMesh = GetOwner()->GetComponentByClass<UStaticMeshComponent>();
}


void AFallingTree::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
}

void AFallingTree::Fall()
{
	
}

