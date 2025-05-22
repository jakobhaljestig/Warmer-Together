// Fill out your copyright notice in the Description page of Project Settings.
#include "ThrowSnowballComponent.h"
#include "CharacterBase.h"

UThrowSnowballComponent::UThrowSnowballComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UThrowSnowballComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UThrowSnowballComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UThrowSnowballComponent::Throw()
{
	if (!SnowballClass || !bCanThrow)
	{
		return;
	}
	
	ACharacter* CharacterOwner = Cast<ACharacter>(GetOwner());
	
	FVector CameraLocation;
	FRotator CameraRotation;
	CharacterOwner->GetController()->GetPlayerViewPoint(CameraLocation, CameraRotation);
	FVector CameraForward = CameraRotation.Vector();

	FVector CharacterForward = CharacterOwner->GetActorForwardVector();

	float AimAngleDegrees = FMath::RadiansToDegrees(acosf(FVector::DotProduct(CameraForward.GetSafeNormal(), CharacterForward.GetSafeNormal())));

	if (AimAngleDegrees > 45)
	{
		UE_LOG(LogTemp, Warning, TEXT("Kastar inte – kameran tittar för långt från karaktärens riktning (vinkel: %.1f°)"), AimAngleDegrees);
		return;
	}
	
	FVector SpawnLocation = CharacterOwner -> GetMesh()->GetSocketLocation("RightHandSocket");
	FRotator SpawnRotation = (CameraRotation.Vector()).Rotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();

	ASnowball* Snowball = GetWorld()->SpawnActor<ASnowball>(SnowballClass, SpawnLocation, SpawnRotation, SpawnParams);

	if (Snowball)
	{
		FVector ThrowDirection = CameraRotation.Vector() + FVector(0, 0, 0.7f); 
		ThrowDirection.Normalize();
		Snowball->ThrowInDirection(ThrowDirection);

		bCanThrow = false;
		
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_ResetThrow, this, &UThrowSnowballComponent::ResetThrow, SnowballInterval, false);
	}
}

void UThrowSnowballComponent::ResetThrow()
{
	bCanThrow = true;
}