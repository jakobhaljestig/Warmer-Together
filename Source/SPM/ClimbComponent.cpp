// Fill out your copyright notice in the Description page of Project Settings.


#include "ClimbComponent.h"

// Sets default values for this component's properties
UClimbComponent::UClimbComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UClimbComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UClimbComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

bool UClimbComponent::CanClimb()
{
	FHitResult Hit;
	return ClimbingInReach(Hit);
}


void UClimbComponent::StartClimb()
{
	//
}

void UClimbComponent::StopClimb()
{
	//
}

bool UClimbComponent::ClimbingInReach (FHitResult& HitResult)
{
	
	return false; 
}