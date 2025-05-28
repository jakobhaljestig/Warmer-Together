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
	
	if (bIsAiming)
	{
		PredictThrowTrajectory();
	}
}



void UThrowSnowballComponent::Aim()
{
	bIsAiming = true;
}


void UThrowSnowballComponent::Throw()
{
	if (!SnowballClass || !bCanThrow || !bIsThrowAreaValid)
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("Throw cancelled"));
		GroundMarker->SetActorHiddenInGame(true);
		bIsAiming = false;
		return;
	}

	ACharacter* CharacterOwner = Cast<ACharacter>(GetOwner());
	if (!CharacterOwner) return;
	
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();

	ASnowball* Snowball = GetWorld()->SpawnActor<ASnowball>(SnowballClass, LastAimLocation, LastAimDirection.Rotation(), SpawnParams);
	if (Snowball)
	{
		Snowball->ThrowInDirection(LastAimDirection);

		bCanThrow = false;
		bIsAiming = false;

		if (GroundMarker)
		{
			GroundMarker->SetActorHiddenInGame(true);
		}

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_ResetThrow, this, &UThrowSnowballComponent::ResetThrow, SnowballInterval, false);
	}
}


void UThrowSnowballComponent::ResetThrow()
{
	bCanThrow = true;
}



void UThrowSnowballComponent::PredictThrowTrajectory()
{
	if (!bIsAiming || !SnowballClass) return;

	ACharacter* CharacterOwner = Cast<ACharacter>(GetOwner());
	if (!CharacterOwner || !CharacterOwner->GetController()) return;

	FVector CameraLocation;
	FRotator CameraRotation;
	CharacterOwner->GetController()->GetPlayerViewPoint(CameraLocation, CameraRotation);

	FVector CameraForward = CameraRotation.Vector();
	FVector CharacterForward = CharacterOwner->GetActorForwardVector();
	float AimAngleDegrees = FMath::RadiansToDegrees(acosf(FVector::DotProduct(CameraForward.GetSafeNormal(), CharacterForward.GetSafeNormal())));

	if (AimAngleDegrees > 45)
	{
		GroundMarker->SetActorHiddenInGame(true);
		bIsThrowAreaValid = false;
		return;
	}

	FVector TraceStart = CameraLocation;
	FVector TraceEnd = TraceStart + (CameraForward * 10000);

	FHitResult Hit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(CharacterOwner);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, QueryParams);
	if (!bHit)
	{
		GroundMarker->SetActorHiddenInGame(true);
		bIsThrowAreaValid = false;
		return;
	}

	// Vi utgår från handen och siktar i kamerans riktning
	FVector StartLocation = CharacterOwner->GetMesh()->GetSocketLocation("RightHandSocket");
	FVector ThrowDirection = CameraRotation.Vector();  // <-- här är fixen!

	FPredictProjectilePathParams PathParams;
	PathParams.StartLocation = StartLocation;
	PathParams.LaunchVelocity = ThrowDirection * GetSnowballSpeed();
	PathParams.ProjectileRadius = 5.f;
	PathParams.MaxSimTime = 2.f;
	PathParams.bTraceWithCollision = true;
	PathParams.SimFrequency = 15.f;
	PathParams.OverrideGravityZ = GetSnowballGravity();
	PathParams.ActorsToIgnore.Add(CharacterOwner);
	// PathParams.DrawDebugType = EDrawDebugTrace::ForDuration; // för felsökning

	FPredictProjectilePathResult PathResult;

	if (UGameplayStatics::PredictProjectilePath(GetWorld(), PathParams, PathResult))
	{
		GroundMarker->SetActorLocation(PathResult.HitResult.Location);
		GroundMarker->SetActorHiddenInGame(false);
		bIsThrowAreaValid = true;

		LastAimDirection = ThrowDirection;
		LastAimLocation = StartLocation;
	}
	else
	{
		GroundMarker->SetActorHiddenInGame(true);
		bIsThrowAreaValid = false;
	}
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