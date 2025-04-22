// Fill out your copyright notice in the Description page of Project Settings.


#include "Health.h"

#include "CharacterBase.h"

// Sets default values for this component's properties
UHealth::UHealth()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Health = MaxHealth;
}


// Called when the game starts
void UHealth::BeginPlay()
{
	Super::BeginPlay();

	
}


// Called every frame
void UHealth::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bFrozen && Health > 0.0f)
	{
		TakeDamage(DeltaTime * HealthDownRate);
	}
}

void UHealth::IsFrozen(bool bShouldBeFrozen)
{
	bFrozen = bShouldBeFrozen;
}


void UHealth::UpdateHealthOnFrozen(float DeltaTime)
{
	Health = Health - DeltaTime * HealthDownRate;
	if (Health <= 0)
	{
		Health = 0;
	}
}

void UHealth::TakeDamage(float Damage)
{

	UE_LOG(LogTemp, Warning, TEXT("Take damage"));
	
	if (Damage <= 0.0f || Health <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Return"));
		return; // om HP är under/ lika med 0 så görs inget
	}

	// decrease HP
	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);

	UE_LOG(LogTemp, Warning, TEXT("decrease HP"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Health: %f"), Health));
	
	if (Health <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player is dead."));

		ACharacterBase* OwnerChar = Cast<ACharacterBase>(GetOwner());
		if (OwnerChar)
		{
			OwnerChar->OnDeath(); //dödslogik från characterbase
		}
	}
}

