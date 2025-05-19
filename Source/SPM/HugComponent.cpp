// Fill out your copyright notice in the Description page of Project Settings.


#include "HugComponent.h"
#include "CharacterBase.h"
#include "Kismet/GameplayStatics.h"

UHugComponent::UHugComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHugComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacterBase>(GetOwner());
}


void UHugComponent::EndHug()
{
	bIsTryingToHug = false;
	UE_LOG(LogTemplateCharacter, Warning, TEXT("Hug has ended"));
}

void UHugComponent::TryHug()
{
	if (!OwnerCharacter) return;

	bIsTryingToHug = true;

	ACharacterBase* Player1 = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(this, 0));
	ACharacterBase* Player2 = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(this, 1));

	if (!Player1 || !Player2 || Player1 == Player2) return;

	UHugComponent* Hug1 = Player1->FindComponentByClass<UHugComponent>();
	UHugComponent* Hug2 = Player2->FindComponentByClass<UHugComponent>();

	if (!Hug1 || !Hug2) return;

	
	if (Hug1->bIsTryingToHug && Hug2->bIsTryingToHug)
	{
		float Distance = FVector::Dist(Player1->GetActorLocation(), Player2->GetActorLocation());

		if (Distance <= HugDistance)
		{
			MovePlayersTogether(Player1, Player2);
			
			Player1->Hug();
			Player2->Hug();
			
			Hug1->bIsTryingToHug = false;
			Hug2->bIsTryingToHug = false;
		}
		else
		{
			UE_LOG(LogTemplateCharacter, Warning, TEXT("Too far apart to hug"));
		}
	}else
	{
		UE_LOG(LogTemplateCharacter, Warning, TEXT("Only one character Hugging"));
	}
}

void UHugComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UHugComponent::MovePlayersTogether(ACharacterBase* P1, ACharacterBase* P2)
{
	if (!P1 || !P2) return;
	
	FVector Location1 = P1->GetActorLocation();
	FVector Location2 = P2->GetActorLocation();
	
	FVector DirectionToP2 = (Location2 - Location1).GetSafeNormal();
	FVector DirectionToP1 = -DirectionToP2;
	
	FRotator RotToP2 = DirectionToP2.Rotation();
	FRotator RotToP1 = DirectionToP1.Rotation();

	P1->SetActorRotation(RotToP2);
	P2->SetActorRotation(RotToP1);
	P2->SetActorLocation(Location1 + P1->GetActorForwardVector() * 40);
}

