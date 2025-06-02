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
	if (!SnowballClass || !bCanThrow)
	{
		return;
	}

	if (GroundMarker)
	{
		GroundMarker->SetActorHiddenInGame(true);
	}

	ACharacter* CharacterOwner = Cast<ACharacter>(GetOwner());
	
	FVector CameraLocation;
	FRotator CameraRotation;
	CharacterOwner->GetController()->GetPlayerViewPoint(CameraLocation, CameraRotation);
	
	FActorSpawnParameters SpawnParams;
	//Ignorerar spelaren som kastar
	SpawnParams.Owner = GetOwner();

	ASnowball* Snowball = GetWorld()->SpawnActor<ASnowball>(SnowballClass, LastAimLocation, LastAimRotation, SpawnParams);

	if (Snowball)
	{
		FVector ThrowDirection = LastThrowDirection;
		ThrowDirection.Normalize();
		Snowball->ThrowInDirection(ThrowDirection, Speed);

		bCanThrow = false;
		bIsAiming = false;
		
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_ResetThrow, this, &UThrowSnowballComponent::ResetThrow, SnowballInterval, false);
	}
}


void UThrowSnowballComponent::ResetThrow()
{
	bCanThrow = true;
}


void UThrowSnowballComponent::PredictThrowTrajectory()
{
	
	if (!SnowballClass || !bCanThrow || !bIsAiming )
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
		GroundMarker->SetActorHiddenInGame(true);
		return;
	}

	//TILL KASTET 
	LastAimLocation = CharacterOwner -> GetMesh()->GetSocketLocation(HandSocketName);
	LastAimRotation = (CameraRotation.Vector()).Rotation();
	LastThrowDirection = CameraRotation.Vector();
	
	FPredictProjectilePathParams PredictParams;
	PredictParams.StartLocation = LastAimLocation;
	PredictParams.LaunchVelocity = LastThrowDirection * Speed;
	//PredictParams.OverrideGravityZ = Gravity;
	PredictParams.MaxSimTime = 2.0f;
	PredictParams.DrawDebugTime = EDrawDebugTrace::ForDuration;
	PredictParams.ActorsToIgnore.Add(CharacterOwner);
	PredictParams.bTraceWithCollision = true;
	PredictParams.bTraceComplex = false;
	PredictParams.DrawDebugTime = 22.0f;

	FPredictProjectilePathResult PredictResult;

	
	bool bHit = UGameplayStatics::PredictProjectilePath(CharacterOwner, PredictParams, PredictResult);

	if (bHit)
	{
		if (GroundMarker)
		{
			FVector HitLocation = PredictResult.HitResult.Location;
			GroundMarker->SetActorLocation(HitLocation);
			GroundMarker->SetActorHiddenInGame(false);
		}
	}
}



//Gammal implementation av kast 
/*void UThrowSnowballComponent::Throw()
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
	
	FVector SpawnLocation = CharacterOwner -> GetMesh()->GetSocketLocation(HandSocketName);
	FRotator SpawnRotation = (CameraRotation.Vector()).Rotation();

	FActorSpawnParameters SpawnParams;
	//Ignorerar spelaren som kastar
	SpawnParams.Owner = GetOwner();

	ASnowball* Snowball = GetWorld()->SpawnActor<ASnowball>(SnowballClass, SpawnLocation, SpawnRotation, SpawnParams);

	if (Snowball)
	{
		FVector ThrowDirection = CameraRotation.Vector() + FVector(0, 0, 0.7f); 
		ThrowDirection.Normalize();
		Snowball->ThrowInDirection(ThrowDirection);

		bCanThrow = false;
		bIsAiming = false;

		
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_ResetThrow, this, &UThrowSnowballComponent::ResetThrow, SnowballInterval, false);
	}
}*/