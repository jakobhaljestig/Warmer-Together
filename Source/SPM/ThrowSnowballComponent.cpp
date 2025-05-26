// Fill out your copyright notice in the Description page of Project Settings.
#include "ThrowSnowballComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CharacterBase.h"

UThrowSnowballComponent::UThrowSnowballComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UThrowSnowballComponent::BeginPlay()
{
	Super::BeginPlay();

	//skapar markör för att sikta
	if (GroundMarkerClass)
	{
		GroundMarker = GetWorld()->SpawnActor<AActor>(GroundMarkerClass);
		GroundMarker->SetActorHiddenInGame(true);
	}
	
}

void UThrowSnowballComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/*if (!bIsAiming || !GroundMarker)
	{
		GroundMarker->SetActorHiddenInGame(true);
		return;
	}

	ACharacter* CharacterOwner = Cast<ACharacter>(GetOwner());
	if (!CharacterOwner) return;

	FVector CameraLocation;
	FRotator CameraRotation;
	CharacterOwner->GetController()->GetPlayerViewPoint(CameraLocation, CameraRotation);

	FVector SpawnLocation = CharacterOwner->GetMesh()->GetSocketLocation("RightHandSocket");
	FVector LaunchVelocity = CameraRotation.Vector() + FVector(0, 0, 0.7f);
	float Speed = GetSnowballSpeed();
	LaunchVelocity = LaunchVelocity.GetSafeNormal() * Speed;

	FPredictProjectilePathParams Params;
	Params.StartLocation = SpawnLocation;
	Params.LaunchVelocity = LaunchVelocity;
	Params.bTraceWithCollision = true;
	Params.ProjectileRadius = 5.f;
	Params.MaxSimTime = 2.f;
	Params.SimFrequency = 15.f;
	Params.TraceChannel = ECC_Visibility;
	Params.ActorsToIgnore.Add(CharacterOwner);
	Params.OverrideGravityZ = GetSnowballGravity();

	FPredictProjectilePathResult Result;

	if (UGameplayStatics::PredictProjectilePath(this, Params, Result))
	{
		GroundMarker->SetActorLocation(Result.HitResult.Location);
		GroundMarker->SetActorHiddenInGame(false);
	}
	else
	{
		GroundMarker->SetActorHiddenInGame(true);
	}*/
}



void UThrowSnowballComponent::Aim()
{
	bIsAiming = true;
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

		if (GroundMarker)
		{
			GroundMarker->SetActorHiddenInGame(true);
		}
		bIsAiming = false;

		
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_ResetThrow, this, &UThrowSnowballComponent::ResetThrow, SnowballInterval, false);
	}
}

void UThrowSnowballComponent::ResetThrow()
{
	bCanThrow = true;
}



float UThrowSnowballComponent::GetSnowballSpeed() const
{
	if (!SnowballClass) return 1000.0f;
	

	const ASnowball* DefaultSnowball = Cast<ASnowball>(SnowballClass->GetDefaultObject());
	return DefaultSnowball ? DefaultSnowball->Speed : 1000.0f;
}

float UThrowSnowballComponent::GetSnowballGravity() const
{
	if (!SnowballClass) return -980.0f;

	const ASnowball* DefaultSnowball = Cast<ASnowball>(SnowballClass->GetDefaultObject());
	return DefaultSnowball ? DefaultSnowball->gravity * -980.0f : -980.0f; // Unreal-gravitation gånger skalan
}
